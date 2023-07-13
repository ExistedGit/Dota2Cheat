#pragma once
#include <json.hpp>
#include <map>
#include <string>
#include "../../SDK/Base/Memory.h"

// Used to process JSON signatures

#define SIGMAP_ENTRY(var) {#var, (void**)&var}
class SignatureDB {
	inline static nlohmann::json Data;
	static void _FindSignature(std::string_view sigName, void** sigVar) {
		enum SignatureAction {
			GetAbsoluteAddress,
			Offset
		};

		if (!Data.contains(sigName))
			return;

		auto& info = Data[sigName];
		std::string sigStr = info["signature"], sigModule = info["module"];
		auto result = Memory::Scan(sigStr, sigModule);

		if (!result)
			return;

		if (info.contains("steps")) {
			for (auto& pair : info["steps"].items()) {
				SignatureAction type = pair.value()[0];
				int value = pair.value()[1];
				switch (type) {
				case GetAbsoluteAddress:
					result = result.GetAbsoluteAddress(value);
					break;
				case Offset:
					result = result.Offset(value);
					break;
				}
			}
		}
		*sigVar = result;
	}
public:

	// Find a singular sig
	static Address FindSignature(const std::string& name) {
		void* ret{};
		_FindSignature(name, &ret);
		return ret;
	}

	// Uses a map of signature names to storage variable pointers to perform a sigscan
	static void ParseSignatures(const std::map <std::string, void**>& signatureMap) {
		for (auto& [sigName, sigVar] : signatureMap)
			_FindSignature(sigName, sigVar);

		bool brokenSig = false;
		for (auto& [sigName, sigVar] : signatureMap) {
			LogF(*sigVar ? LP_DATA : LP_ERROR, "{}: {}", sigName, *sigVar);
			if (!(*sigVar))
				brokenSig = true;
		}
		if (brokenSig)
			system("pause");
	}

	static void LoadSignaturesFromFile(const std::string& url) {
		if (std::ifstream fin(url); fin.is_open()) {
			LogF(LP_INFO, "Loading signatures from {}\n", url);
			Data = nlohmann::json::parse(fin);
			fin.close();
		}
	};

	//static bool LoadSignaturesFromNetwork(const std::string& url) {
	//	LogF(LP_INFO, "Loading signatures from {}\n", url);

	//	std::stringstream out;
	//	CURL* curl = curl_easy_init();
	//	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteRemoteString);
	//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
	//	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	//	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Dota2Cheat/0.1");
	//	CURLcode CURLresult = curl_easy_perform(curl);
	//	curl_easy_cleanup(curl);

	//	if (CURLresult != CURLE_OK)
	//		// std::cout << "FAILED TO LOAD SIGNATURES FROM " << url << ", USING LOCAL COPY" << '\n';
	//		return false;
	//	Data = nlohmann::json::parse(out.str());
	//	return true;
	//}
};