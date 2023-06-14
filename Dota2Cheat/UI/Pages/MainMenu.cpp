#include "MainMenu.h"

void Pages::MainMenu::Draw() {
	ImGui::Begin("Main", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (CheatGui::Button("Scripting"))
		scriptMenuVisible = !scriptMenuVisible;

	if (scriptMenuVisible) {

		ImGui::Begin("Scripting");
		ImGui::InputTextMultiline("Lua script", scriptBuf, 4096, ImVec2(300, 500));
		if (CheatGui::Button("Execute"))
			ctx.lua.script(scriptBuf);

		ImGui::End();
	}
	static auto& cl_particle_log_creates = Interfaces::CVar->CVars["cl_particle_log_creates"].m_pVar->value.boolean;
#ifdef _DEBUG
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("cl_particle_log_creates", &cl_particle_log_creates);

	ImGui::InputText("Sound name", &uiSoundBuf);

	if (ImGui::Button("PLAY SOUND")) {
		void* unk;
		Signatures::PlayUISoundScript(&unk, uiSoundBuf.c_str(), 1);
	}

	if (ImGui::Button("Log Entities"))
		LogEntities();


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

	if (ctx.gameStage == GameStage::IN_GAME)
		if (CheatGui::Button("Circle drawing"))
			circleMenuVisible = !circleMenuVisible;

	if (ImGui::TreeNode("AutoAccept")) {
		ImGui::Checkbox("Enabled", &Config::AutoAccept::Enabled);
		if (Config::AutoAccept::Enabled) {

			ImGui::SliderInt("Delay", &Config::AutoAccept::Delay, 0, 6);
			ImGui::Checkbox("Send telegram notifications", &Config::AutoAccept::SendTelegramNotifications);
			if (Config::AutoAccept::SendTelegramNotifications) {
				ImGui::InputUInt64("Telegram ID", &Config::API::TelegramID);
				ImGui::SameLine(); HelpMarker("First, start the bot at t.me/dotacheatnotifybot\nThen get your ID at t.me/getmyid_bot");
			}

		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Bars")) {
		ImGui::Checkbox("Manabars", &Config::Bars::ManaBar);
		ImGui::Checkbox("HP amount on healthbar", &Config::Bars::HPNumbers);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("TP Tracker")) {
		ImGui::Text("Shows enemy teleports on the map");

		ImGui::Checkbox("Enabled", &Config::TPTracker::Enabled);
		ImGui::SameLine(); HelpMarker("In case of Boots of Travel it only shows the position the receiver was at when the TP started");
		ImGui::SliderInt("Fade duration", &Config::TPTracker::FadeDuration, 3, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Particle Maphack")) {
		ImGui::Text("Shows enemies on both the map and the screen when they create a particle with position information in it(like casting a spell).");

		ImGui::Checkbox("Enabled", &Config::ParticleMapHack::Enabled);
		ImGui::SliderInt("Fade duration", &Config::ParticleMapHack::FadeDuration, 3, 10);
		ImGui::TreePop();
	}

	if (ImGui::CollapsingHeader("Changer")) {

		ImGui::InputText("Rich Presence status", &rpStatusBuf);
		if (CheatGui::Button("Apply status"))
			GameSystems::RichPresence->SetRPStatus(rpStatusBuf.c_str());


		ImGui::Checkbox("Unlock Dota Plus", &Config::Changer::UnlockDotaPlus);
		if(ImGui::Checkbox("Unlock emoticons", &Config::Changer::UnlockEmoticons)) {
			static auto dota_hud_chat_enable_all_emoticons = Interfaces::CVar->CVars["dota_hud_chat_enable_all_emoticons"].m_pVar;
			dota_hud_chat_enable_all_emoticons->value.boolean = Config::Changer::UnlockEmoticons;
		};
		// https://github.com/SK68-ph/Shadow-Dance-Menu
		ImGui::Combo("Weather", &Config::Changer::WeatherListIdx, WeatherList, IM_ARRAYSIZE(WeatherList));

		// credits to the screenshot https://yougame.biz/threads/283404/
		// and to Wolf49406 himself
		// should've figured out it's controlled by a convar like the weather :)
		ImGui::Combo("River paint", &Config::Changer::RiverListIdx, RiverList, IM_ARRAYSIZE(RiverList));
	};
	if (ImGui::TreeNode("Snatcher")) {
		ImGui::Checkbox("Bounty runes", &Config::AutoPickUpRunes);
		ImGui::Checkbox("Aegis", &Config::AutoPickUpAegis);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("AbilityESP")) {
		ImGui::Checkbox("Enable", &Config::AbilityESP::Enabled);
		ImGui::Checkbox("Include allied heroes", &Config::AbilityESP::ShowAllies);
		ImGui::Checkbox("Show decimals in cooldowns", &Config::AbilityESP::ShowCooldownDecimals);
		ImGui::SliderFloat("UI scale", &Config::AbilityESP::UIScale, 0.7f, 1.4f, "%.1f");
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Indicators")) {
		ImGui::Checkbox("Speed indicator", &Config::Indicators::Speed);
		ImGui::SameLine(); HelpMarker("Shows an icon near the enemy signifying whether you are faster or slower");
		ImGui::Checkbox("Kill indicator", &Config::Indicators::Kill);
		ImGui::SameLine(); HelpMarker(
			"If your hero has a nuke spell, its icon will be displayed near the enemy's healthbar, signifying whether you can kill them with it. "
			"If you can't, it shows how much health is left until you can kill them");
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
		ImGui::Checkbox("Show enemy projectile trajectory", &Config::ShowLinearProjTrajectory);
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
		ImGui::Text("Drops/moves items that add stats before using items that give mana/HP");
		ImGui::Checkbox("Enabled", &Config::ManaAbuse::Enabled);

		static const char* Modes[] = {
			"Drop",
			"Move"
		};
		ImGui::Combo("Mode", &Config::ManaAbuse::Mode, Modes, IM_ARRAYSIZE(Modes));
		if (Config::ManaAbuse::Mode == 0) {
			ImGui::SliderInt("Safety radius", &Config::ManaAbuse::SafetyRadius, 300, 1200);
			ImGui::SameLine(); HelpMarker("If there are any enemies within the radius, the Drop mode will not drop items.");
		}
		ImGui::TreePop();
	}
	ImGui::Checkbox("Blink Revealer", &Config::BlinkRevealer);
	ImGui::SameLine(); HelpMarker("Reveals the location of Queen of Pain's or Antimage's blink");

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

	ImGui::SliderFloat("Camera distance", &Config::CameraDistance, 1200, 3000, "%.0f");

	if (CheatGui::Button("EXIT", ImVec2(100, 50)))
		glfwSetWindowShouldClose(window_menu, 1);

	ImGui::End();

	if (circleMenuVisible) {
		ImGui::Begin("C I R C L E S", &circleMenuVisible, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::InputInt("Circle radius", &Config::CircleRadius, 1, 10);
		ImGui::ColorEdit3("Circle RGB", &Config::CircleRGB.x);

		if (CheatGui::Button("Draw circle")) {
			Vector color = Config::CircleRGB * 255;
			Vector radius{ static_cast<float>(Config::CircleRadius), 255, 0 };

			auto particle = GameSystems::ParticleManager->CreateParticle(
				"particles/ui_mouseactions/selected_ring.vpcf",
				PATTACH_ABSORIGIN_FOLLOW,
				ctx.localHero
			).particle
				->SetControlPoint(1, color)
				->SetControlPoint(2, radius)
				->SetControlPoint(3, Vector::Zero);
		}
		ImGui::End();
	}

}
