#include "Bytehook.h"
#include <MinHook.h>

bool hooks::UnhookAll() {
	bool res = true;
	for (const auto& hook : installed)
		res = res && Unhook(hook.target);
	return res;
}

bool hooks::Unhook(void* target) {
	auto it = std::find_if(installed.begin(), installed.end(), [target](auto data) { return data.target == target; });

	if (it == installed.end()) {
		LogFW("Tried to unhook nonexistent target {}", target);
		return false;
	}

	bool res = MH_RemoveHook(target) == MH_OK;
	if (res) installed.erase(it);
	else LogFE("Could not unhook {}!", it->name);
	return res;
}

bool hooks::Hook(void* target, void* detour, void* original, const char* name) {
	bool result =
		MH_CreateHook(target, detour, (LPVOID*)original) == MH_OK
		&& MH_EnableHook(target) == MH_OK;

	if (result)
		installed.push_back(HookData{ name, target });
	else
		LogFE("Could not hook {}!", name);

	return result;
}

bool hooks::HookData::SetEnabled(bool val) {
	bool res = false;
	if (enabled != val) {
		if (val)
			res = MH_EnableHook(target) == MH_OK;
		else
			res = MH_DisableHook(target) == MH_OK;
	}

	enabled = val;
	return res;
}
