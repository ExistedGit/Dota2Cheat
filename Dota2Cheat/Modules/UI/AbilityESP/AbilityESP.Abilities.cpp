#include "AbilityESP.h"
#include "../../../Utils/Drawing.h"
#include "../../../CheatSDK/Shaders.h"

constexpr unsigned VISIBLE_ABILITY_COUNT = 6;

void Modules::M_AbilityESP::DrawHeroAbilities(const CHero* hero) {
	const CAbility* queue[VISIBLE_ABILITY_COUNT]{ nullptr };
	int queueSize = 0;
	auto abilities = hero->GetAbilities();

	// Forming the ability render queue

	// EXPLANATION FOR CHECKS \/

	// client.dll CDOTA_Hud_AbilityPanel vtable ->
	// CDOTA_Hud_AbilityPanel::CDOTA_Hud_AbilityPanel(constructor, 3 params) ->
	// CDOTA_Hud_AbilityList::UpdateAbilityList(via xref "Ability%d", contains three conditional calls to above ctor ->
	// conditionals preceding the calls(a lot of vfuncs which boil down to simple getters)

	// checks for all kinds of attributes that would mean an ability is not part of the main GUI

	constexpr auto check3 = [](DOTAAbilityDefinition_t* def) -> bool {
		if (!def->m_bInnate)
			return false;

		auto behavior = def->m_iAbilityBehavior;
		auto v2 = *(float**)(def + 0x128);
		bool passive = (behavior & DOTA_ABILITY_BEHAVIOR_PASSIVE) != 0;
		bool v4 = !v2 || *v2 == 0.0;
		bool notLearnable = (behavior & DOTA_ABILITY_BEHAVIOR_NOT_LEARNABLE) != 0 || def->m_iMaxLevel <= 1; // not learnable or maxlevel 0
		bool innateUI = (behavior & DOTA_ABILITY_BEHAVIOR_INNATE_UI) != 0;
		return innateUI || (passive && notLearnable);
		};

	for (const CHandle<CAbility> hAbility : abilities) {
		
		if (
			!hAbility.IsValid()
			)
			continue;

		const CAbility* ability = *hAbility;
		if (ability->IsHidden())
			continue;

		auto def = ability->GetDefinition();
		auto barType = ability->Member<AbilityBarType_t>(Netvars::C_DOTABaseAbility::m_nAbilityBarType);
		if (
			!def
			|| def->m_iAbilityType == ABILITY_TYPE_ATTRIBUTES
			|| (def->m_iAbilityBehavior & DOTA_ABILITY_BEHAVIOR_INNATE_UI)
			|| !def->m_bOnCastbar
			|| check3(def)
			|| barType != ABILITY_BAR_TYPE_MAIN
			)
			continue;

		queue[queueSize++] = ability;

		if (queueSize >= VISIBLE_ABILITY_COUNT) break;
	}

	const float iconSize = ScaleVar(AbilityIconSize);
	const float outlineThickness = 1;
	const ImVec2 outlineSize{ outlineThickness, outlineThickness };
	const int levelCounterHeight = 8;
	const float rounding = Config::AbilityESP::Rounding / 100.f * iconSize / 2;
	auto DrawList = ImGui::GetForegroundDrawList();
	auto lvlCounterType = (LevelCounterType)Config::AbilityESP::LevelCounterType;

	constexpr auto getCooldown = [](const CAbility* ent)->float {
		if (
			ent->GetCooldown() != 0 || // if on cooldown
			(ent->GetCharges() == 0 && // or has 0 charges and a charge cooldown
				ent->GetChargeRestoreCooldown() > 0)
			)
			return ent->GetCooldown() // choosing either of these cooldowns, since they're mutually exclusive
			? ent->GetCooldown()
			: ent->GetChargeRestoreCooldown();

		return 0.0f;
		};

	for (int idx = 0; idx < queueSize; idx++) {
		const CAbility* ability = queue[idx];

		const auto icon = assets.spellIcons.Load(ability->GetAbilityTextureName(Config::AbilityESP::ApplyIconModifiers));

		float cooldown = getCooldown(ability);

		ImVec2 drawPos = HeroData[(CNPC*)hero].W2S;
		drawPos.x -= (queueSize - 1) * iconSize / 2.0f;
		drawPos.y += 30;

		if (!ability)
			continue;

		// Top-Left, Bottom-Right and Center points of ability icon
		ImVec2 imgXY1, imgXY2, imgCenter;
		float centeringOffset = -outlineThickness + iconSize / 2;
		{
			int idxOffset = idx * iconSize;

			imgXY1 = drawPos - ImVec2{ centeringOffset - idxOffset, centeringOffset };
			imgXY2 = drawPos + ImVec2{ centeringOffset + idxOffset, centeringOffset };
			imgCenter = imgXY1 + ImVec2(centeringOffset, centeringOffset);
		}


		DrawList->AddRectFilled(
			imgXY1 - outlineSize,
			imgXY2 + outlineSize,
			ImColor(0, 0, 0), rounding);


		// Black outline by default
		ImU32 outlineColor = ImColor{ 0,0,0 };

		// Orange for autocast
		if (ability->Member<bool>(Netvars::C_DOTABaseAbility::m_bAutoCastState))
			outlineColor = ImColor(255, 191, 0);

		// Green for toggle
		if (ability->IsToggled())
			outlineColor = ImColor(3, 0xAC, 0x13);

		DrawList->AddRectFilled(
			imgXY1 - outlineSize,
			imgXY2 + outlineSize,
			outlineColor, rounding);

		// If the ability is not learned icon turns grey 
		// If there is no mana to cast the icon turns blue
		if (ability->GetLevel() == 0) DrawList->AddCallback(GreyscaleShaderCallback, nullptr);
		else if (ability->GetManaCost() > hero->GetMana()) DrawList->AddCallback(NoManaShaderCallback, nullptr);

		// Ability icon itself
		DrawList->AddImageRounded(icon, imgXY1, imgXY2, { 0,0 }, { 1,1 }, ImColor{ 255,255,255 }, rounding);

		DrawList->AddCallback(ImDrawCallback_ResetRenderState, nullptr);

		// Not learned or on cooldown => darker icon
		if (
			ability->GetLevel() == 0 ||
			cooldown > 0
			)
			DrawList->AddRectFilled(imgXY1, imgXY2, ImColor(0, 0, 0, 130), rounding);

		if (cooldown) {
			int cdFontSize = iconSize - ScaleVar(8);
			bool decimals = Config::AbilityESP::ShowCooldownDecimals;
			if (floorf(ability->GetCooldown()) >= 100) {
				cdFontSize = iconSize - ScaleVar(12);
				decimals = false;
			}

			if (decimals)
				cdFontSize = iconSize - ScaleVar(14);
			else cooldown = ceil(cooldown);

			auto textPos = imgCenter - ImVec2{ 0, cdFontSize / 2.f };
			// Draws the cooldown
			DrawTextForeground(DrawData.GetFont("Monofonto", cdFontSize),
				std::vformat(decimals ? "{:.1f}" : "{:.0f}", std::make_format_args(cooldown)),
				textPos,
				cdFontSize,
				ImColor{ 255,255,255 },
				true);
		}

		// Channeling countdown
		if (const float channelTime = ability->Member<float>(Netvars::C_DOTABaseAbility::m_flChannelStartTime); channelTime != 0) {
			float indicatorHeight = ScaleVar(4);
			auto channelLength = ability->GetLevelSpecialValueFor("AbilityChannelTime");
			int fontSize = ScaleVar(18);

			DrawTextForeground(DrawData.GetFont("Monofonto", fontSize),
				std::format("{:.1f}", channelLength - (CGameRules::Get()->GetGameTime() - channelTime)),
				ImVec2(imgXY1.x + centeringOffset, imgXY1.y - fontSize - 2 - indicatorHeight),
				fontSize,
				ImVec4(1, 1, 1, 1),
				true);

			float indicatorWidth = abs(imgXY2.x - imgXY1.x) * (1 - ((CGameRules::Get()->GetGameTime() - channelTime) / channelLength));
			DrawList->AddRectFilled(
				ImVec2(imgXY1.x, imgXY1.y - indicatorHeight),
				ImVec2(imgXY1.x + indicatorWidth, imgXY1.y), ImColor(1, 1, 1, 0.7), rounding);
		}
		// If it's being cast
		else if (ability->Member<bool>(Netvars::C_DOTABaseAbility::m_bInAbilityPhase)) {
			float castPoint = ability->GetLevelSpecialValueFor("AbilityCastPoint"),
				castStartTime = ability->Member<float>(Netvars::C_DOTABaseAbility::m_flCastStartTime);
			int fontSize = ScaleVar(18);
			float indicatorWidth = abs(imgXY1.x - imgXY2.x) * ((CGameRules::Get()->GetGameTime() - castStartTime) / castPoint);

			ImVec2 clip_max = ImVec2(imgXY1.x + indicatorWidth, imgXY2.y);

			DrawList->PushClipRect(imgXY1, clip_max, true);

			DrawList->AddRectFilled(imgXY1, imgXY2, ImColor(0.f, 1.f, 0.f, 0.5f), rounding);

			DrawList->PopClipRect();

			DrawTextForeground(DrawData.GetFont("Monofonto", fontSize),
				std::format("{:.1f}", castPoint - (CGameRules::Get()->GetGameTime() - castStartTime)),
				imgXY1 + ImVec2(centeringOffset, -fontSize - 2),
				fontSize,
				ImColor(0, 255, 60),
				true);
		}

		// Charge counters

		if (
			ability->GetCharges()
			||
			(ability->GetCharges() == 0 && ability->GetChargeRestoreCooldown() >= 0)
			) {

			DrawChargeCounter(ability->GetCharges(), imgXY1);
		}


		// Level counters

		if (ability->GetLevel() == 0 || ability->GetLevel() == 1 && ability->GetMaxLevel() == 1)
			continue;

		// Bars don't look very good on high levels
		if (lvlCounterType == LevelCounterType::Bars && ability->GetMaxLevel() > 4)
			lvlCounterType = LevelCounterType::NumberBasic;

		switch (lvlCounterType) {
		case LevelCounterType::NumberImmersive:
			DrawLevelCounterImmersive(ability, ImVec2{ imgXY1.x, imgXY2.y });
			break;
		case LevelCounterType::NumberBasic:
			DrawLevelCounterBasic(ability, ImVec2{ imgXY1.x, imgXY2.y });
			break;
		case LevelCounterType::Bars:
			DrawLevelBars(ability,
				ImVec2{ imgXY1.x, imgXY2.y - 3 }, { imgXY2.x, imgXY2.y + 3 });
			break;
		}
	}
}

void Modules::M_AbilityESP::DrawLevelCounterImmersive(const CDOTABaseAbility* ability, const ImVec2& pos) {
	int lvl = ability->GetLevel();

	const auto clrLvlOutline = ImColor(231, 210, 146);
	const auto clrLvlBackground = ImColor(40, 15, 1);
	const ImVec2 outlinePadding(1, 1);
	int counterScale = ScaleVar(16);
	int counterTextScale = ScaleVar(14);

	ImVec2 counterSize(counterScale, counterScale);
	ImVec2 imgXY1 = pos - counterSize / 2, imgXY2 = pos + counterSize / 2;

	DrawRectFilled(imgXY1, counterSize, clrLvlBackground);
	DrawRect(
		imgXY1 + outlinePadding,
		counterSize - outlinePadding * 2,
		clrLvlOutline
	);

	DrawTextForeground(DrawData.GetFont("Monofonto", counterTextScale), std::to_string(lvl),
		ImVec2{ pos.x, pos.y - counterTextScale / 2 - counterTextScale % 2 },
		counterTextScale,
		clrLvlOutline,
		true);
}

void Modules::M_AbilityESP::DrawLevelCounterBasic(const CDOTABaseAbility* ability, const ImVec2& pos)
{
	int counterScale = ScaleVar(26);
	DrawTextForeground(DrawData.GetFont("Monofonto", counterScale - 2), std::to_string(ability->GetLevel()),
		ImVec2(pos.x, pos.y - (ScaleVar(32) - 2) / 2),
		counterScale - 2,
		ImColor{ 255,255,255 },
		true);
}

void Modules::M_AbilityESP::DrawLevelBars(const CDOTABaseAbility* ability, const ImVec2& xy1, const ImVec2& xy2) {
	const auto clrLearned = ImColor(193, 254, 0);

	int lvl = ability->GetLevel(), maxLvl = ability->GetMaxLevel();

	const auto elemWidth = (xy2.x - xy1.x) / maxLvl;
	auto DrawList = ImGui::GetForegroundDrawList();
	const ImVec2 outline{ 1.f,1.f };
	for (int i = 0; i < lvl; ++i) {
		auto barXY1 = ImVec2(xy1.x + i * elemWidth, xy1.y);
		auto barXY2 = ImVec2(xy1.x + (i + 1) * elemWidth, xy2.y);
		DrawList->AddRectFilled(
			barXY1,
			barXY2,
			ImColor{ 0,0,0 },
			elemWidth / 4
		);
		DrawList->AddRectFilled(
			barXY1 + outline,
			barXY2 - outline,
			clrLearned,
			elemWidth / 4
		);
	}
}