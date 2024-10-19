#pragma once
#include "../CheatSDK/include.h"

namespace Hooks {
#ifdef _DEBUG
	// this nasty thing prevents peaceful debugging
	// I should also report them to Uncle Bob for violating the SRP
	// (who the hell puts both the check and the popup in one function?)

	inline void* oBShowRestrictedAddonPopup;
	inline bool hkBShowRestrictedAddonPopup() {
		return false;
	}
#endif 

	inline void* oSaveSerializedSOCache{};
	inline void* hkSaveSerializedSOCache(void* thisptr) {
		auto result = ((decltype(&hkSaveSerializedSOCache))oSaveSerializedSOCache)(thisptr);
		//Modules::SkinChanger.DeleteSOCacheFiles();
		d2c.SaveConfig();
		//{
		//	std::ofstream fout(d2c.cheatFolderPath + "\\config\\inventory.json");
		//	Config::cfg.SaveEquippedItems(fout);
		//	fout.close();
		//}
		return result;
	}
}