#include "AbilityESP.h"
#include "../../../Utils/Drawing.h"
#include "../../../CheatSDK/Shaders.h"
#include <format>

void Modules::M_AbilityESP::UpdateHeroRenderData(const CHero* h) {
	

	const auto getCooldown = [](const CAbility* ent)->float {
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

	RenderData& rd = renderData[h->GetIndex()];

	rd.isDrawable = CanDraw(h);

	if (!rd.isDrawable) return;

	rd.HealthbarW2S = HeroData[(CHero*)h].HealthbarW2S;
	rd.W2S = HeroData[(CHero*)h].W2S;
	rd.hasStatus = h->Member<uint64_t>(Netvars::C_DOTA_BaseNPC::m_nUnitDebuffState) != 0;
#pragma region Abilities
	rd.abilityCount = 0;
	auto abilities = h->GetAbilities();

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

		RenderData::Ability item{
			.icon = ability->GetAbilityTextureName(Config::AbilityESP::ApplyIconModifiers),
			.level = ability->GetLevel(),
			.maxLevel = ability->GetMaxLevel(),
			.cooldown = getCooldown(ability),
			.toggled = ability->IsToggled(),
			.autocast = ability->Member<bool>(Netvars::C_DOTABaseAbility::m_bAutoCastState),
			.noMana = h->GetMana() < ability->GetManaCost()
		};

		if (ability->GetCharges()) {
			item.charges = ability->GetCharges();
		}
		else if (ability->GetChargeRestoreCooldown() > 0) {
			item.charges = 0;
		}

		if (ability->Member<bool>(Netvars::C_DOTABaseAbility::m_bInAbilityPhase)) {
			float castPoint = ability->GetLevelSpecialValueFor("AbilityCastPoint");
			float castStartTime = ability->Member<float>(Netvars::C_DOTABaseAbility::m_flCastStartTime);

			item.castPoint = castPoint;
			item.castTime = CGameRules::Get()->GetGameTime() - castStartTime;
		}

		if (
			const float channelStartTime = ability->Member<float>(Netvars::C_DOTABaseAbility::m_flChannelStartTime);
			channelStartTime != 0
			) {
			auto channelLength = ability->GetLevelSpecialValueFor("AbilityChannelTime");

			item.channelingTime = CGameRules::Get()->GetGameTime() - channelStartTime;
			item.channelingTimeTotal = channelLength;
		}

		rd.abilities[rd.abilityCount++] = item;

		if (rd.abilityCount >= VISIBLE_ABILITY_COUNT) break;
	}
#pragma endregion

	const auto updateItemData = [&](RenderData::Item& out, const CHero* hero, int slot) {
		const auto hItem = hero->GetItems()[slot];
		out.valid = hItem.IsValid();

		if (!out.valid) return;

		// + 5 to remove "item_" prefix
		out.icon = hItem->GetAbilityTextureName(Config::AbilityESP::ApplyIconModifiers) + 5;

		out.cooldown = getCooldown(hItem);
		out.toggled = hItem->IsToggled();

		if (hItem->GetCharges()) {
			out.charges = hItem->GetCharges();
		}
		else if (hItem->GetChargeRestoreCooldown() > 0) {
			out.charges = 0;
		}

		};

	for (int i = 0; i < VISIBLE_ITEM_COUNT; i++) {
		updateItemData(rd.items[i], h, i);
	}

	updateItemData(rd.tpSlot, h, 15);
	updateItemData(rd.neutralSlot, h, 16);
}

bool Modules::M_AbilityESP::CanDraw(const CHero* hero) const {
	bool ret =
#ifndef _DEBUG
		hero != ctx.localHero &&
#endif
		hero->GetIdentity()
		&& !hero->GetIdentity()->IsDormant()
		&& !hero->IsIllusion()
		&& hero->GetLifeState() == 0
		&& IsPointOnScreen(HeroData[(CNPC*)hero].W2S);
	if (!Config::AbilityESP::ShowAllies)
		// I wish they made &&= an operator
		ret = ret && !hero->IsSameTeam(ctx.localHero);
	return ret;
}

void Modules::M_AbilityESP::DrawHeroAbilities(const RenderData& rd) const {
	const float iconSize = ScaleVar(AbilityIconSize);
	const float outlineThickness = 1;
	const ImVec2 outlineSize{ outlineThickness, outlineThickness };
	const int levelCounterHeight = 8;
	const float rounding = Config::AbilityESP::Rounding / 100.f * iconSize / 2;
	auto dl = ImGui::GetBackgroundDrawList();
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

	for (int idx = 0; idx < rd.abilityCount; idx++) {
		const auto& ability = rd.abilities[idx];

		const ID3D11ShaderResourceView* icon = assets.spellIcons.Load(ability.icon);

		float cooldown = ability.cooldown;

		ImVec2 drawPos = rd.W2S;
		drawPos.x -= (rd.abilityCount - 1) * iconSize / 2.0f;
		drawPos.y += 30;


		// Top-Left, Bottom-Right and Center points of ability icon
		ImVec2 imgXY1, imgXY2, imgCenter;
		float centeringOffset = -outlineThickness + iconSize / 2;
		{
			int idxOffset = idx * iconSize;

			imgXY1 = drawPos - ImVec2{ centeringOffset - idxOffset, centeringOffset };
			imgXY2 = drawPos + ImVec2{ centeringOffset + idxOffset, centeringOffset };
			imgCenter = imgXY1 + ImVec2(centeringOffset, centeringOffset);
		}


		dl->AddRectFilled(
			imgXY1 - outlineSize,
			imgXY2 + outlineSize,
			ImColor(0, 0, 0), rounding);


		// Black outline by default
		ImU32 outlineColor = ImColor{ 0,0,0 };

		// Orange for autocast
		if (ability.autocast)
			outlineColor = ImColor(255, 191, 0);

		// Green for toggle
		if (ability.toggled)
			outlineColor = ImColor(3, 0xAC, 0x13);

		dl->AddRectFilled(
			imgXY1 - outlineSize,
			imgXY2 + outlineSize,
			outlineColor, rounding);

		// If the ability is not learned icon turns grey 
		// If there is no mana to cast the icon turns blue
		if (ability.level == 0) dl->AddCallback(GreyscaleShaderCallback, nullptr);
		else if (ability.noMana) dl->AddCallback(NoManaShaderCallback, nullptr);

		// Ability icon itself
		dl->AddImageRounded((ImTextureID)icon, imgXY1, imgXY2, { 0,0 }, { 1,1 }, ImColor{ 255,255,255 }, rounding);

		dl->AddCallback(ImDrawCallback_ResetRenderState, nullptr);

		// Not learned or on cooldown => darker icon
		if (
			ability.level == 0 ||
			cooldown > 0
			)
			dl->AddRectFilled(imgXY1, imgXY2, ImColor(0, 0, 0, 130), rounding);

		if (cooldown) {
			int cdFontSize = iconSize - ScaleVar(8);
			bool decimals = Config::AbilityESP::ShowCooldownDecimals;
			if (floorf(ability.cooldown) >= 100) {
				cdFontSize = iconSize - ScaleVar(12);
				decimals = false;
			}

			if (decimals)
				cdFontSize = iconSize - ScaleVar(14);
			else cooldown = ceil(cooldown);

			auto textPos = imgCenter - ImVec2{ 0, cdFontSize / 2.f };
			// Draws the cooldown
			ImDrawText(DrawData.GetFont("Monofonto", cdFontSize),
				std::vformat(decimals ? "{:.1f}" : "{:.0f}", std::make_format_args(cooldown)),
				textPos,
				cdFontSize,
				ImColor{ 255,255,255 },
				true);
		}

		// Channeling countdown
		if (ability.channelingTime != 0) {
			float indicatorHeight = ScaleVar(4);
			int fontSize = ScaleVar(18);

			ImDrawText(DrawData.GetFont("Monofonto", fontSize),
				std::format("{:.1f}", ability.channelingTimeTotal - ability.channelingTime),
				ImVec2(imgXY1.x + centeringOffset, imgXY1.y - fontSize - 2 - indicatorHeight),
				fontSize,
				ImVec4(1, 1, 1, 1),
				true);

			float indicatorWidth = abs(imgXY2.x - imgXY1.x) * (1 - (ability.channelingTime / ability.channelingTimeTotal));
			dl->AddRectFilled(
				ImVec2(imgXY1.x, imgXY1.y - indicatorHeight),
				ImVec2(imgXY1.x + indicatorWidth, imgXY1.y), ImColor(1.f, 1.f, 1.f, 0.7f));
		}
		// If it's being cast
		else if (ability.castTime != 0) {
			int fontSize = ScaleVar(18);
			float indicatorWidth = abs(imgXY1.x - imgXY2.x) * (ability.castTime / ability.castPoint);

			ImVec2 clip_max = ImVec2(imgXY1.x + indicatorWidth, imgXY2.y);

			dl->PushClipRect(imgXY1, clip_max, true);

			dl->AddRectFilled(imgXY1, imgXY2, ImColor(0.f, 1.f, 0.f, 0.5f), rounding);

			dl->PopClipRect();

			ImDrawText(DrawData.GetFont("Monofonto", fontSize),
				std::format("{:.1f}", ability.castPoint - ability.castTime),
				imgXY1 + ImVec2(centeringOffset, -fontSize - 2),
				fontSize,
				ImColor(0, 255, 60),
				true);
		}

		// Charge counters

		if (ability.charges) {
			DrawChargeCounter(*ability.charges, imgXY1);
		}

		// Level counters

		if (ability.level == 0 || ability.level == 1 && ability.maxLevel == 1)
			continue;

		// Bars don't look very good on high levels
		if (lvlCounterType == LevelCounterType::Bars && ability.maxLevel > 4)
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

void Modules::M_AbilityESP::DrawHeroItems(const RenderData& rd) const {
	auto itemType = (ItemPanelType)Config::AbilityESP::ItemPanelType;

	if (itemType == ItemPanelType::Sequence)
		DrawItemSequences(rd);
	else
		DrawItemGrids(rd);
}

void Modules::M_AbilityESP::DrawChargeCounterBasic(int charges, const ImVec2& pos) const {
	auto textSize = ScaleVar(20);
	ImDrawText(
		DrawData.GetFont("Monofonto", textSize),
		std::to_string(charges),
		ImVec2(pos.x, pos.y - textSize / 2),
		textSize,
		ImVec4(1, 1, 1, 1),
		true);
}

void Modules::M_AbilityESP::DrawChargeCounterImmersive(int charges, const ImVec2& pos) const {
	auto dl = ImGui::GetBackgroundDrawList();

	int counterScale = ScaleVar(16) / 2;
	int counterTextScale = ScaleVar(14);
	// Green outline
	dl->AddCircleFilled(pos, counterScale + 1, ImColor(135, 214, 77));
	// Gray core
	dl->AddCircleFilled(pos, counterScale, ImColor(0.2f, 0.2f, 0.2f, 1.0f));
	auto textSize = ScaleVar(20);
	ImDrawText(
		DrawData.GetFont("Monofonto", counterTextScale),
		std::to_string(charges),
		ImVec2(pos.x, pos.y - counterTextScale / 2 - counterTextScale % 2),
		counterTextScale,
		ImVec4(1, 1, 1, 1),
		true);
}


void Modules::M_AbilityESP::Draw() {
	if (!Config::AbilityESP::Enabled)
		return;

	MTM_LOCK;

	for (const auto& [id, rd] : renderData) {
		if (!rd.isDrawable) continue;

		DrawHeroAbilities(rd);
		DrawHeroItems(rd);
	}
}

void Modules::M_AbilityESP::OnEntityAdded(const EntityWrapper& ent)
{
	MTM_LOCK;
	if(ent.type != EntityType::Hero) return;

	renderData[ent->GetIndex()] = RenderData();
}

void Modules::M_AbilityESP::OnEntityRemoved(const EntityWrapper& ent)
{
	MTM_LOCK;
	if(ent.type != EntityType::Hero) return;

	renderData.erase(ent->GetIndex());
}
