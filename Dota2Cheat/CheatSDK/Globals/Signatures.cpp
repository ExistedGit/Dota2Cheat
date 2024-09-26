#include "Signatures.h"

void Signatures::FindSignatures() {
	CMsg = Memory::GetExport("tier0.dll", "Msg");
	CMsgColor = Memory::GetExport("tier0.dll", "?ConColorMsg@@YAXAEBVColor@@PEBDZZ");

	SignatureDB::ParseSignatures(NamedSignatures);

	size_t padding = 0;
	{
		using PairType = decltype(NamedSignatures)::value_type;
		auto longestNameElem = std::max_element(NamedSignatures.begin(), NamedSignatures.end(),
			[](const PairType& a, const PairType& b) {
				return a.first.length() < b.first.length();
			});

		if (longestNameElem != NamedSignatures.end()) padding = longestNameElem->first.size();
	}


	LogI("[ FUNCTIONS ]");
	{
		std::lock_guard lock(mLogging);
		for (const auto& [name, pVal] : NamedSignatures) {
			LogPrefix prefix = pVal ? LP_DATA : LP_ERROR;
			SetConsoleColor();
			SetLogColor(prefix);

			std::cout << std::left << std::setw(padding) << name << " = " << std::hex << std::uppercase << *pVal << std::endl;
		}
	}
}
