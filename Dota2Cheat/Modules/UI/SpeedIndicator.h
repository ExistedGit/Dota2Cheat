#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"
#include "../../Utils/Drawing.h"

namespace Hacks {
	class SpeedIndicator {
	public:
		void Draw() {
			if (!Config::Indicators::Speed)
				return;

			int mySpeed = ctx.assignedHero->GetIdealSpeed();
			for (auto& hero : ctx.heroes) {
				if (hero->IsSameTeam(ctx.assignedHero)
					|| !hero->IsTargetable()
					|| hero->IsIllusion())
					continue;

				auto heroPos = hero->GetPos();
				heroPos.z += hero->Member<int>(Netvars::C_DOTA_BaseNPC::m_iHealthBarOffset);

				auto barPos = WorldToScreen(heroPos);

				int enemySpeed = hero->GetIdealSpeed();

				auto drawPos = barPos - ImVec2{ 38, 105 };
				ImColor img_color = { 255, 255, 255, 255 };
				if (enemySpeed > mySpeed)
					img_color = { 255, 0, 0, 255 };
				else if(enemySpeed < mySpeed)
					img_color = { 0, 255, 0, 255 };

				ImGui::GetForegroundDrawList()->AddImage(
					(void*)texManager.GetNamedTexture("icon_speed"),
					drawPos + ImVec2{ 2, 2 },
					drawPos + ImVec2(25, 25),
					{ 0.0f, 0.0f },
					{ 1.0f, 1.0f },
					img_color);
			}
		}
	};
}
namespace Modules {
	inline Hacks::SpeedIndicator SpeedIndicator{};
}