#pragma once
#include <imgui/imgui.h>
#include "../../Config.h"
#include "../../Input.h"

namespace Pages {
	namespace MainMenu {

		inline char scriptBuf[4096]{};
		inline std::string rpStatusBuf;
		inline bool scriptMenuVisible = false;
		inline bool circleMenuVisible = false;

		inline int debugEntIdx = 0;

		inline void Draw(GLFWwindow* window) {
			ImGui::Begin("Main", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::Button("Scripting"))
				scriptMenuVisible = !scriptMenuVisible;

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

			if (ImGui::TreeNode("AutoAccept")) {
				ImGui::Checkbox("Enabled", &Config::AutoAccept::Enabled);
				ImGui::SliderInt("Delay", &Config::AutoAccept::Delay, 0, 6);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TP Tracker")) {
				ImGui::Text("Shows enemy teleports on the map(currently not working for Boots of Travel)");
				ImGui::Checkbox("Enabled", &Config::TPTracker::Enabled);
				ImGui::TreePop();
			}
			if (ImGui::CollapsingHeader("Changer")) {

				ImGui::InputText("Rich Presence status", &rpStatusBuf);
				if (ImGui::Button("Apply status"))
					GameSystems::RichPresence->SetRPStatus(rpStatusBuf.c_str());
				

				ImGui::Checkbox("Unlock Dota Plus", &Config::Changer::UnlockDotaPlus);
				ImGui::Checkbox("Unlock emoticons", &Config::Changer::UnlockEmoticons);
				// https://github.com/SK68-ph/Shadow-Dance-Menu
				ImGui::ListBox(
					"Change weather",
					&Config::Changer::WeatherListIdx,
					UIState::WeatherList,
					IM_ARRAYSIZE(UIState::WeatherList),
					4);
				
				// credits to the screenshot https://yougame.biz/threads/283404/
				// and to Wolf49406 himself
				// should've figured out it's controlled by a convar like the weather :)
				ImGui::ListBox(
					"River paint",
					&Config::Changer::RiverListIdx,
					UIState::RiverList,
					IM_ARRAYSIZE(UIState::RiverList),
					4);
			};
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
				ImGui::Text("Hold [Alt] to switch from AbilityESP to ItemESP");
				ImGui::Checkbox("Enable", &Config::AbilityESP::Enabled);
				ImGui::Checkbox("Include allied heroes", &Config::AbilityESP::ShowAllies);
				ImGui::Checkbox("Show decimals in cooldowns", &Config::AbilityESP::ShowCooldownDecimals);
				ImGui::Checkbox("Show manabars", &Config::AbilityESP::ShowManabars);
				ImGui::SliderFloat("Icon scale", &Config::AbilityESP::UIScale, 0.75f, 2.0f, "%.1f");

				ImGui::Checkbox("Crop stashed icons", &Config::AbilityESP::CropStashItems);
				ImGui::SameLine(); HelpMarker("Stashed items will be displayed like in Dota itself");
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Illusion coloring")) {
				ImGui::Checkbox("Enable", &Config::IllusionColoring::Enabled);
				ImGui::ColorEdit3("Color", &Config::IllusionColoring::Color.x);
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
			if (ImGui::TreeNode("AutoHeal")) {
				ImGui::Text("Uses Magic Stick/Wand, Holy Locket and/or Faerie Fire on low health");
				ImGui::Checkbox("Enabled", &Config::AutoHeal::Enabled);
				ImGui::SliderFloat("Faerie Fire HP Treshold", &Config::AutoHeal::FaerieFireHPTreshold, 1, 100, "%.1f");
				ImGui::SliderFloat("Wand Locket HP Treshold", &Config::AutoHeal::WandHPTreshold, 1, 100, "%.1f");
				ImGui::SliderInt("Minimum wand charges", &Config::AutoHeal::WandMinCharges, 1, 20);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("AutoMidas")) {
				ImGui::Checkbox("Enabled", &Config::AutoMidas::Enabled);
				ImGui::SliderInt("XP Treshold", &Config::AutoMidas::XPTreshold, 40, 90);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Mana/HP Abuse")) {
				ImGui::Text("Drops items that add stats before using items that give mana/HP");
				ImGui::Checkbox("Enabled", &Config::ManaAbuse::Enabled);
				ImGui::TreePop();
			}

			ImGui::Checkbox("AutoDodge", &Config::AutoDodge::Enabled);
			ImGui::SameLine(); HelpMarker("Can use Manta Style, Bottled Illusion Rune");

			ImGui::Checkbox("Show all particles", &Config::RenderAllParticles);
			ImGui::SameLine(); HelpMarker("Renders any possible particle, even in FoW");
			ImGui::Checkbox("Last hit marker", &Config::LastHitMarker);
			ImGui::SameLine(); HelpMarker("Shows a dot on creeps when you can last hit/deny them");


			ImGui::Checkbox("Show enemy projectile trajectory", &Config::WarnLinearProjectiles);
			ImGui::SameLine(); HelpMarker("Draws a red line for things like Mirana's arrow");

			ImGui::Checkbox("Perfect Blink", &Config::PerfectBlink);
			ImGui::Checkbox("Prevent bad casts", &Config::BadCastPrevention);
			ImGui::SameLine(); HelpMarker("Detect if there are enemy heroes within range of some AOE skills to prevent bad casts. e.g, Chronosphere, Black hole, Reverse polarity, etc.");

			ImGui::Checkbox("Redirect illusion casts", &Config::CastRedirection);
			ImGui::SameLine(); HelpMarker("You cast something on an illusion - it aims for the real hero(if they're in range, of course)");

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
						ctx.assignedHero
					).particle
						->SetControlPoint(1, color)
						->SetControlPoint(2, radius)
						->SetControlPoint(3, Vector::Zero);
				}
				ImGui::End();
			}

		}
	}
}