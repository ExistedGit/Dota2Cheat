#include "SignatureDB.h"
#include <Base/Logging.h>
#include <fstream>

Address SignatureDB::FindSignature(const std::string& sigName) {
	if (!Data.contains(sigName))
		return nullptr;

	auto& info = Data[sigName];

	auto res = Memory::ScanBinary(info.data, info.dll);
	if (!res)
		return nullptr;

	// Implicitly dereferencing call opcodes
	if (info.data[0] == '\xE8')
		res = res.GetAbsoluteAddress(1);

	if (info.steps.size()) {
		for (const auto& step : info.steps) {
			switch (step.action) {
			case Signature::Action::GetAbsoluteAddress: res = res.GetAbsoluteAddress(step.parameter); break;
			case Signature::Action::Offset: res = res.Offset(step.parameter); break;
			}
		}
	}
	return res;
}


// Use a map of signature names to variable pointers to perform a sigscan

void SignatureDB::ParseSignatures(const std::map<std::string, void**>& signatureMap) {
	bool brokenSig = false;
	for (const auto& [sigName, sigVar] : signatureMap) {
		*sigVar = FindSignature(sigName);
	}
}

void SignatureDB::LoadSignaturesFromFile(const std::string& url) {
	std::ifstream fin(url);
	if (!fin.is_open())
		return;

	nlohmann::json data;
	try {
		data = nlohmann::json::parse(fin);
	}
	catch (std::exception e) {
		LogFE("Could not load signatures from {}: {}", url, e.what());
	}

	for (const auto& [moduleName, sigBlock] : data.items()) {
		for (const auto& [sigName, sigData] : sigBlock.items()) {

			std::string signature = sigData.type() == nlohmann::json::value_t::string ? sigData : sigData["signature"];

			std::vector<Signature::Step> steps;

			if (sigData.contains("steps")) {
				steps.reserve(sigData["steps"].size());
				for (const auto& step : sigData["steps"].items()) {
					steps.push_back(
						Signature::Step{
							.action = step.value()[0],
							.parameter = step.value()[1]
						}
					);
				}
			}

			Data[sigName] = Signature{
				.data = IDAtoBinary(signature),
				.dll = moduleName,
				.steps = steps
			};
		}
	}

	fin.close();

}
