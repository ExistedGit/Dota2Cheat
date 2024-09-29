#pragma once
#include <iostream>
#include <vector>
#include "Logging.h"

namespace hooks {
	struct HookData {
		std::string name;
		void* target;
		bool enabled = true;

		bool SetEnabled(bool val);
	};

	inline std::vector<HookData> installed;

	bool UnhookAll();
	bool Unhook(void* target);
	bool Hook(void* target, void* detour, void* original, const char* name);;
}

#define HOOKFUNC(func) hooks::Hook(func, &hk##func, &o##func, #func)
#define ORIGCALL(func) ((decltype(&hk##func))(o##func))