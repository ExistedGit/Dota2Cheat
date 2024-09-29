#include "Signatures.h"
#include "../Tables.h"
#include <Base/Logging.h>
#include "SignatureDB.h"

void Signatures::FindSignatures() {
	CMsg = Memory::GetExport("tier0.dll", "Msg");
	CMsgColor = Memory::GetExport("tier0.dll", "?ConColorMsg@@YAXAEBVColor@@PEBDZZ");

	SignatureDB::ParseSignatures(bindings);

	size_t padding = 0;
	{
		using PairType = decltype(bindings)::value_type;
		auto longestNameElem = std::max_element(bindings.begin(), bindings.end(),
			[](const PairType& a, const PairType& b) {
				return a.first.length() < b.first.length();
			});

		if (longestNameElem != bindings.end()) padding = longestNameElem->first.size();
	}

	
	LogI("[ FUNCTIONS ]");
	{
		std::lock_guard lock(mLogging);
		for (const auto& [name, pVal] : bindings) {
			LogPrefix prefix = pVal ? LP_DATA : LP_ERROR;
			SetConsoleColor();
			SetLogColor(prefix);

			std::cout << std::left << std::setw(padding) << name << " = " << std::hex << std::uppercase << *pVal << std::endl;
		}
	}
}
