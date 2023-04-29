#pragma once
#include "../Utils/Drawing.h"
#include "../../CheatSDK/include.h"
#include "../../DebugFunctions.h"

namespace Pages {
	namespace MainMenu {
		const char* WeatherList[] = { "Default", "Winter", "Rain", "MoonBeam", "Pestilence", "Harvest", "Sirocco", "Spring", "Ash", "Aurora" };
		const char* RiverList[] = { "Default", "Chrome", "Dry", "Slime", "Oil", "Electric", "Potion", "Blood" };

		inline char scriptBuf[4096]{};
		inline std::string rpStatusBuf;
		inline bool scriptMenuVisible = false;
		inline bool circleMenuVisible = false;

		inline int debugEntIdx = 0;

		inline void Draw() {
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
				ImGui::Text("Shows enemy teleports on the map");

				ImGui::Checkbox("Enabled", &Config::TPTracker::Enabled);
				ImGui::SameLine(); HelpMarker("In case of Boots of Travel it only shows the position the receiver was at when the TP started");
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
					WeatherList,
					IM_ARRAYSIZE(WeatherList),
					4);

				// credits to the screenshot https://yougame.biz/threads/283404/
				// and to Wolf49406 himself
				// should've figured out it's controlled by a convar like the weather :)
				ImGui::ListBox(
					"River paint",
					&Config::Changer::RiverListIdx,
					RiverList,
					IM_ARRAYSIZE(RiverList),
					4);
			};
			if (ImGui::TreeNode("Snatcher")) {
				ImGui::Checkbox("Bounty runes", &Config::AutoPickUpRunes);
				//ImGui::Checkbox("Aegis", &Config::AutoPickUpAegis);
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
				ImGui::SliderFloat("UI scale", &Config::AbilityESP::UIScale, 0.7f, 1.5f, "%.1f");

				ImGui::Checkbox("Crop stashed icons", &Config::AbilityESP::CropStashItems);
				ImGui::SameLine(); HelpMarker("Stashed items will be displayed like in Dota itself");
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Indicators")) {
				ImGui::Checkbox("Speed indicator", &Config::Indicators::Speed);
				ImGui::SameLine(); HelpMarker("Shows an icon near the enemy signifying whether you are faster or slower");
				ImGui::Checkbox("Kill indicator", &Config::Indicators::Kill);
				ImGui::SameLine(); HelpMarker(
					"If your hero has a nuke spell, its icon will be displayed near the enemy's healthbar, signifying whether you can kill them with it."
					"If not, it shows how much health is left until you can kill them");
				ImGui::SliderFloat("Kill indicator scale", &Config::Indicators::KillScale, 1, 1.4, "%.1f");
				//	ImGui::Checkbox("Top bars", &Config::UIOverhaul::TopBars);
			//	ImGui::SameLine(); HelpMarker("Shows HP and Mana bars for enemies in the top bar. Like pressing Alt does for your allies");
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
				ImGui::Checkbox("Show enemy projectile trajectory", &Config::WarnLinearProjectiles);
				ImGui::SameLine(); HelpMarker("Draws a red line for things like Mirana's arrow");

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Modifier revealer")) {

				if (ImGui::Checkbox("Targeted spells", &Config::ModifierRevealer::TargetedSpells) &&
					!Config::ModifierRevealer::TargetedSpells)
					Modules::TargetedSpellHighlighter.OnDisableTargetedSpells();
				ImGui::SameLine(); HelpMarker("Assassinate, Charge of Darkness, Track");

				if (ImGui::Checkbox("True Sight", &Config::ModifierRevealer::TrueSight)
					&& !Config::ModifierRevealer::TrueSight)
					Modules::TrueSightESP.OnDisable();

				if (ImGui::Checkbox("Linken Sphere", &Config::ModifierRevealer::LinkenSphere) &&
					!Config::ModifierRevealer::LinkenSphere)
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



			ImGui::Checkbox("Perfect Blink", &Config::PerfectBlink);
			ImGui::Checkbox("Prevent bad casts", &Config::BadCastPrevention);
			ImGui::SameLine(); HelpMarker("Detect if there are enemy heroes within range of some AOE skills to prevent bad casts. e.g, Chronosphere, Black hole, Reverse polarity, etc.");

			ImGui::Checkbox("Redirect illusion casts", &Config::CastRedirection);
			ImGui::SameLine(); HelpMarker("You cast something on an illusion - it aims for the real hero(if they're in range, of course)");

			ImGui::SliderFloat("Camera distance", &Config::CameraDistance, 1200, 3000, "%.1f");

			if (ImGui::Button("EXIT", ImVec2(100, 50)))
				glfwSetWindowShouldClose(window_menu, 1);

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