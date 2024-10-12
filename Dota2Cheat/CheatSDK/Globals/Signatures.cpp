#include "Signatures.h"
#include "../Tables.h"
#include <Base/Logging.h>
#include "SignatureDB.h"
#include <Interfaces/CEngineServiceMgr.h>
#include <Interfaces/CInputService.h>

void Signatures::FindSignatures() {
	CEventDispatcher::_UnregisterEventListener_Abstract = Address(CInputService::Get()->GetVFunc(4)).Offset(0x6C).GetAbsoluteAddress(1);
	CEventDispatcher::_RegisterEventListener_Abstract = Address(CInputService::Get()->GetVFunc(3)).Offset(0x280).GetAbsoluteAddress(1);

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
