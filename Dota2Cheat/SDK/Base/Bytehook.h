#pragma once
#include <MinHook.h>
#include <iostream>
#include "Logging.h"

inline bool HookFunc(void* func, void* detour, void* original, const char* name) {
	bool result = MH_CreateHook(func, detour,
		(LPVOID*)original) == MH_OK &&
		MH_EnableHook(func) == MH_OK;

	if (!result)
		LogF(LP_ERROR, "Could not hook {}()!", name);

	return result;
};
#define HOOKFUNC(func) HookFunc(func, &hk##func, &o##func, #func)