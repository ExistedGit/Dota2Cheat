#pragma once
#include "../SDK/pch.h"

namespace Hooks {
	typedef void(*RunScriptFn)(VClass*, VClass*, const char*, const char*, int, int, bool);
	inline RunScriptFn oRunScript;

	void RunTimer(VClass** Panel);
	void hkRunScript(VClass* thisptr, VClass* panel, const char* entireJSCode, const char* pathToXMLContext, int int1, int int2, bool alreadyCompiled);
}