#pragma once
#include "../../SDK/Panorama/CPanel2D.h"
#include "../../SDK/Panorama/CUIPanel.h"
#include "Interfaces.h"

namespace Panorama {
	inline CDOTA_Hud* DotaHud{};
	inline CDOTA_Hud_ErrorMsg* ErrorMessages{};

	inline void FindPanels() {
		// Panorama's HUD root
		for (auto& node : Interfaces::UIEngine->GetPanelList<4096>()) {
			auto uiPanel = node.uiPanel;
			if (!uiPanel->GetId())
				continue;
			std::string_view id = uiPanel->GetId();
			if (id != "DotaHud")
				continue;

			DotaHud = (CDOTA_Hud*)uiPanel;
			break;
		}

		ErrorMessages = (CDOTA_Hud_ErrorMsg*)DotaHud->FindChildWithIdTraverse("ErrorMessages")->GetPanel2D();
	}
}