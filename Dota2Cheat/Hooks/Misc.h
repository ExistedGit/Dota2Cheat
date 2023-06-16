#pragma once
#include "../SDK/pch.h"
#include "../CheatSDK/Config.h"
#include "../CheatSDK/DrawData.h"
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
			std::ofstream fout(ctx.cheatFolderPath + "\\config\\base.json");
			Config::cfg.SaveConfig(fout);
			fout.close();
		}
		{
			std::ofstream fout(ctx.cheatFolderPath + "\\config\\inventory.json");
			Config::cfg.SaveEquippedItems(fout);
			fout.close();
		}
		return result;
	}

	inline Signatures::C_DOTA_ProjectileManager__AddTrackingProjectileFn oC_DOTA_ProjectileManager__AddTrackingProjectile{};
	inline void* hkC_DOTA_ProjectileManager__AddTrackingProjectile(C_DOTA_ProjectileManager* thisptr, void* unk) {
		if (!GameSystems::ProjectileManager)
			GameSystems::ProjectileManager = thisptr;

		return oC_DOTA_ProjectileManager__AddTrackingProjectile(thisptr, unk);
	}
}