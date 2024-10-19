#pragma once
#include <Dota2SDK.h>

namespace Panorama {
	inline CDOTA_Hud* DotaHud{};
	inline CDOTA_Hud_ErrorMsg* ErrorMessages{};

	inline void FindPanels() {
		// Panorama's HUD root
		auto panels = CUIEngine::Get()->GetPanelList<4096>();
		DotaHud = (CDOTA_Hud*)std::find_if(
			panels.begin(), panels.end(), 
			[](const auto& node) { 
				return node.uiPanel->GetId() && std::string_view(node.uiPanel->GetId()) == "DotaHud"; 
			})
			->uiPanel;

		ErrorMessages = (CDOTA_Hud_ErrorMsg*)DotaHud->FindChildByIdTraverse("ErrorMessages")->GetPanel2D();
	}
}