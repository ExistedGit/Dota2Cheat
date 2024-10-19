#pragma once
#include <Base/Bytehook.h>

namespace Hooks {
	void InstallHooks();

	void InstallAuxiliaryHooks();

	// Removes any custom, non-MinHook hooks
	void RemoveAuxiliaryHooks();

	// Reloads every hook
	inline void Reload() {
		hooks::UnhookAll();
		InstallHooks();
	}
}
