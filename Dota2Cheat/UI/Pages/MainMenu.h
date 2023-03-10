#pragma once
#include <imgui/imgui.h>
#include "../../Config.h"
#include "../../Input.h"

namespace Pages {
	namespace MainMenu {

		inline char scriptBuf[4096]{};
		inline char addrBuf[4096]{};
		inline bool scriptMenuVisible = false;
		inline bool circleMenuVisible = false;

		inline int debugEntIdx = 0;

		inline void Display(GLFWwindow* window) {
			ImGui::Begin("Main", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::Button("Scripting"))
				scriptMenuVisible = !scriptMenuVisible;

			//ImGui::End();


			if (scriptMenuVisible) {

				ImGui::Begin("Scripting");
				ImGui::InputTextMultiline("Lua script", scriptBuf, 4096, ImVec2(300, 500));
				if (ImGui::Button("Execute"))
					ctx.lua.script(scriptBuf);

				ImGui::End();
			}
#ifdef _DEBUG
			ImGui::Begin("Debug functions", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

			
			if (ImGui::Button("Log Entities")) {
				LogEntities();
			}
			if (ImGui::Button("Log Inventory")) {
				auto selected = ctx.localPlayer->GetSelectedUnits();
				auto ent = (CDOTABaseNPC*)Interfaces::EntitySystem->GetEntity(selected[0]);
				LogInvAndAbilities(ent);
			}

			if (ImGui::Button("Log Modifiers")) {
				auto selected = ctx.localPlayer->GetSelectedUnits();
				auto ent = (CDOTABaseNPC*)Interfaces::EntitySystem->GetEntity(selected[0]);
				LogModifiers(ent);
			}

			ImGui::End();
#endif // _DEBUG



			if (ImGui::Button("Circle drawing"))
				circleMenuVisible = !circleMenuVisible;


			if (ImGui::TreeNode("Visuals")) {
				// https://github.com/SK68-ph/Shadow-Dance-Menu
				ImGui::ListBox(
					"Change weather",
					&Config::WeatherListIdx,
					UIState::WeatherList,
					IM_ARRAYSIZE(UIState::WeatherList),
					4);

				// credits to the screenshot https://yougame.biz/threads/283404/
				// and to Wolf49046 himself
				// should've figured out it's controlled by a convar like the weather :)
				ImGui::ListBox(
					"River paint",
					&Config::RiverListIdx,
					UIState::RiverList,
					IM_ARRAYSIZE(UIState::RiverList),
					4);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Auto-pickup")) {
				ImGui::Checkbox("Bounty runes", &Config::AutoPickUpRunes);
				ImGui::Checkbox("Aegis", &Config::AutoPickUpAegis);
				ImGui::TreePop();
			}

			//if (ImGui::TreeNode("Visible by Enemy")) {
			//	ImGui::Checkbox("Show HIDDEN/DETECTED text", &Config::VBEShowText);
			//	ImGui::Checkbox("Show a circle under the hero when visible", &Config::VBEShowParticle);
			//	ImGui::TreePop();
			//}
			if (ImGui::TreeNode("AbilityESP")) {
				ImGui::Checkbox("Enable", &Config::AbilityESPEnabled);
				ImGui::Checkbox("Include allied heroes", &Config::AbilityESPShowAllies);
				ImGui::SliderFloat("Icon scale", &Config::AbilityESPIconScale, 0.75f, 2.0f, "%.1f");
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Illusion coloring")) {
				ImGui::Checkbox("Enable", &Config::IllusionColoring);
				ImGui::ColorEdit3("Color", &Config::IllusionColor.x);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Show enemy spells")) {
				ImGui::Checkbox("Point spells", &Config::ShowEnemyPointSpells);
				ImGui::SameLine(); HelpMarker("Sunstrike, Torrent, Light Strike Array");

				if (ImGui::Checkbox("Targeted spells", &Config::ShowEnemyTargetedSpells) &&
					!Config::ShowEnemyTargetedSpells)
					Modules::TargetedSpellHighlighter.OnDisableTargetedSpells();
				ImGui::SameLine(); HelpMarker("Assassinate, Charge of Darkness, Track");

				if (ImGui::Checkbox("Show Linken Sphere", &Config::ShowLinkenSphere) &&
					!Config::ShowLinkenSphere)
					Modules::TargetedSpellHighlighter.OnDisableLinken();
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("AutoWand")) {
				ImGui::Checkbox("Auto-use Faerie Fire and Magic Stick", &Config::AutoWandEnabled);
				ImGui::SliderFloat("Faerie Fire HP Treshold", &Config::AutoHealFaerieFireHPTreshold, 0, 100, "%.1f");
				ImGui::SliderFloat("Magic Stick/Wand/Holy Locket HP Treshold", &Config::AutoHealWandHPTreshold, 0, 100, "%.1f");
				ImGui::SliderInt("Minimum charges", &Config::AutoHealWandMinCharges, 1, 20);

				ImGui::TreePop();
			}
			ImGui::Checkbox("AutoDodge", &Config::AutoDodge);
			ImGui::SameLine(); HelpMarker("Can use Manta Style, Bottled Illusion Rune and Naga's Mirror Image");


			ImGui::Checkbox("Show all particles", &Config::RenderAllParticles);
			ImGui::SameLine(); HelpMarker("Renders any possible particle, even in FoW");

			ImGui::Checkbox("Show enemy projectile trajectory", &Config::WarnLinearProjectiles);
			ImGui::SameLine(); HelpMarker("Draws a red line for things like Mirana's arrow");

			ImGui::Checkbox("Redirect illusion casts", &Config::CastRedirection);
			ImGui::SameLine(); HelpMarker("You cast something on an illusion - it aims for the real hero(if they're in range, of course)");

			ImGui::Checkbox("Auto-use Hand of Midas", &Config::AutoMidasEnabled);
			ImGui::Checkbox("Auto-buy Tome of Knowledge", &Config::AutoBuyTome);
			ImGui::SliderFloat("Camera distance", &Config::CameraDistance, 1200, 3000, "%.1f");


			if (ImGui::Button("EXIT", ImVec2(100, 50)))
				glfwSetWindowShouldClose(window, 1);

			ImGui::End();

			if (circleMenuVisible) {
				ImGui::Begin("C I R C L E S", &circleMenuVisible, ImGuiWindowFlags_AlwaysAutoResize);
				ImGui::InputInt("Circle radius", &Config::CircleRadius, 1, 10);
				ImGui::ColorEdit3("Circle RGB", &Config::CircleRGB.x);

				if (ImGui::Button("Draw circle")) {
					Vector color = Config::CircleRGB * 255;
					Vector radius{ static_cast<float>(Config::CircleRadius), 255, 0 };

					auto particle = GameSystems::ParticleManager->CreateParticle(
						"particles/ui_mouseactions/selected_ring.vpcf",
						PATTACH_ABSORIGIN_FOLLOW,
						(CBaseEntity*)ctx.assignedHero
					).particle
						->SetControlPoint(1, &color)
						->SetControlPoint(2, &radius)
						->SetControlPoint(3, &Vector::Zero);
				}
				ImGui::End();
			}




			//{
			//	for (auto& proj : UtilityModules::ProjectileTracker.projectiles) {
			//		int x = 0, y = 0;
			//		Signatures::Scripts::WorldToScreen(&proj.pos, &x, &y, nullptr);
			//		y += 10;
			//		DrawTextForeground(window, defaultFont, "\\/", ImVec2(x, y), 20, Color{ 255,0,0,255 }, true);
			//	}
			//}
			//if (ctx.IsInMatch && Config::VBEShowText)
			//	DrawTextForeground(window, vbeFont, UIState::HeroVisibleToEnemy ? "DETECTED" : "HIDDEN", ImVec2(1920 / 2, 1080 * 3 / 4), 80.0f, Color(200, 200, 200, 255), true);


#ifdef _DEBUG
		//if (ctx.assignedHero) {
		//	int x = 0, y = 0;
		//	auto vec = ctx.assignedHero->GetForwardVector(500);
		//	Signatures::Scripts::WorldToScreen(&vec, &x, &y, nullptr);
		//	int size = 10;
		//	DrawRect(window, ImVec2(x - size, y - size), ImVec2(size, size), ImVec4(1, 0, 0, 1));
		//}
#endif // _DEBUG


		}
	}
}