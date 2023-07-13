#pragma once
#include "../pch.h"
#include "../CheatSDK/include.h"
#include "../Modules/Hacks/AutoAccept.h"

// AutoAccept logic
// We wait until the accept popup appears, and then BAM! Game accepted

namespace Hooks {
	inline void* oRunScript;

	inline void hkRunScript(VClass* thisptr, VClass* panel, const char* entireJSCode, const char* pathToXMLContext, void* unk) {
		if (strstr(pathToXMLContext, "popup_accept_match.xml"))
			Modules::AutoAccept.AcceptMatch();

		((decltype(&hkRunScript))oRunScript)(thisptr, panel, entireJSCode, pathToXMLContext, unk);
	}

}