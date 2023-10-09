#pragma once
#include "../pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	// Adds manabars to enemies and displays their HP as a number
	inline class BarAugmenter {
		bool* ManaBarSwitch{};

		bool CanDraw(CDOTABaseNPC_Hero* hero);
		void DrawHPNumbers(CDOTABaseNPC_Hero* hero) {
			const int fontSize = 14;
			if (!CanDraw(hero))
				return;

			ImVec2 drawPos =
				HeroData[hero].HealthbarW2S;

			drawPos.y -= 34;

			DrawTextForeground(DrawData.GetFont("Monofonto", fontSize), std::to_string(hero->GetHealth()), drawPos, fontSize, ImColor{ 255,255,255 }, true);
		}
		void UpdateManabarSwitch() {
			if (*ManaBarSwitch != Config::Bars::ManaBars)
				Memory::Patch(ManaBarSwitch, { Config::Bars::ManaBars });
		}
		//void DrawManabar(CDOTABaseNPC_Hero* hero)
		//{
		//	// Fine-tuned values
		//	// idk why it's this strange
		//	constexpr static ImVec2 manabarSize{ 101, 6 };

		//	if (!CanDraw(hero))
		//		return;

		//	ImVec2 drawPos = HeroData[hero].HealthbarW2S + ImVec2(4, -16);
		//	// Background
		//	DrawRectFilled(
		//		drawPos - ImVec2(110, manabarSize.y) / 2,
		//		manabarSize, ImVec4(0, 0, 0, 1));
		//	// Manabar
		//	DrawRectFilled(
		//		drawPos - ImVec2(110, manabarSize.y) / 2 + ImVec2(1, 1),
		//		ImVec2(manabarSize.x * (hero->GetMana() / hero->GetMaxMana()) - 2, manabarSize.y - 2), ImColor(0.f, 0.5f, 1.f));
		//}
	public:
		void Init() {


			//               test    rbx, rbx
			//               jz      short loc_18175C626
			//               lea     rdx, aMedusaManaShie; "medusa_mana_shield"
			//               mov     rcx, rbx
			//               call    sub_1814FCE80
			//               test    rax, rax
						//	 jnz     short loc_18175C634
						//	 mov[rsp + 0D10h + var_CA0], 0 <--------- THIS INSTRUCTION
						//   jmp     short loc_18175C639
						//	 lea     rbx, [r15 - 0A20h]
						//	 mov[rsp + 0D10h + var_CA0], 1 <--------- as opposed to this one

			// Then we just dynamically patch the mov to emulate the presence of Medusa's mana shield

			ManaBarSwitch = Memory::Scan("75 0E C6 44 24", "client.dll").Offset(4 + 2);
			UpdateManabarSwitch();
		}
		void Draw() {
			UpdateManabarSwitch();
			//if (Config::Bars::ManaBars)
			//	EntityList.ForEach<CDOTABaseNPC_Hero>([this](auto* hero) { DrawManabar(hero); });
			if (Config::Bars::HPNumbers)
				EntityList.ForEach<CDOTABaseNPC_Hero>([this](auto* hero) { DrawHPNumbers(hero); });
		}
	} BarAugmenter{};
}