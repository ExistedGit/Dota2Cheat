#pragma once
#include "../../../pch.h"
#include "../../../CheatSDK/include.h"
#include "../../../Utils/Drawing.h"

namespace Modules {
	// Created together with Wolf49406
	inline class M_SpeedIndicator {
		void DrawIndicatorFor(CDOTABaseNPC_Hero* hero) {
			if (hero->IsSameTeam(ctx.localHero)
				|| !hero->IsTargetable()
				|| hero->IsIllusion()
				|| !IsEntityOnScreen(hero))
				return;

			const static auto icon = assets.Directory("hud/reborn").Load("icon_speed", "psd");
			int mySpeed = ctx.localHero->GetIdealSpeed();

			auto barPos = HeroData[hero].HealthbarW2S;
			int enemySpeed = hero->GetIdealSpeed();

			auto drawPos = barPos - ImVec2{ 105,38 };
			ImColor img_color = { 255, 255, 255, 255 };
			if (enemySpeed > mySpeed)
				img_color = { 255, 0, 0, 255 };
			else if (enemySpeed < mySpeed)
				img_color = { 0, 255, 0, 255 };

			ImGui::GetForegroundDrawList()->AddImage(
				icon,
				drawPos + ImVec2{ 2, 2 },
				drawPos + ImVec2(25, 25),
				{ 0.0f, 0.0f },
				{ 1.0f, 1.0f },
				img_color);
		}
	public:
		void Draw();
	} SpeedIndicator;
}