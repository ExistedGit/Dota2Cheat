#pragma once
#include <MinHook.h>
#include <iostream>

inline bool HookFunc(void* func, void* detour, void* original, const char* name) {
	bool result = MH_CreateHook(func, detour,
		(LPVOID*)original) == MH_OK &&
		MH_EnableHook(func) == MH_OK;
	if (!result)
		std::cout << "Could not hook" << name << "()!\n";
	return result;
};
#define HOOKFUNC(func) HookFunc(func, &hk##func, &o##func, #func)