#pragma once
#include "../SDK/pch.h"
#include "../CheatSDK/Config.h"
#include "../CheatSDK/Data/DrawData.h"

namespace Hooks {

	inline CVarSystem::CVarCallbackFn oOnHUDFlipped;
	inline void* hkOnHUDFlipped(const CVarID& id, int unk1, const CVarValue* val, const CVarValue* old_val) {
		DrawData.IsHUDFlipped = val->boolean;
		return oOnHUDFlipped(id, unk1, val, old_val);
	};

	inline Signatures::SaveSerializedSOCacheFn oSaveSerializedSOCache{};
	inline void* hkSaveSerializedSOCache(void* thisptr) {
		auto result = oSaveSerializedSOCache(thisptr);
		Modules::SkinChanger.DeleteSOCacheFiles();
		{
			std::ofstream fout(d2c.cheatFolderPath + "\\config\\base.json");
			Config::cfg.SaveConfig(fout);
			fout.close();
		}
		{
			std::ofstream fout(d2c.cheatFolderPath + "\\config\\inventory.json");
			Config::cfg.SaveEquippedItems(fout);
			fout.close();
		}
		return result;
	}
}