#pragma once
#include <MinHook.h>
#include <iostream>

inline void HookFunc(void* func, void* detour, void* original, const char* name) {
	if (MH_CreateHook(func, detour,
		(LPVOID*)original) != MH_OK ||
		MH_EnableHook(func) != MH_OK)
		std::cout << "Could not hook" << name << "()!\n";
};