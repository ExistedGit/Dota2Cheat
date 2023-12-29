#include "AbilityESP.h"
#include <format>

void Modules::M_AbilityESP::SubscribeHeroes() {
	// Clean up anything unneeded
	for (auto it = EnemyAbilities.begin(); it != EnemyAbilities.end(); )
	{
		if (EntityList.IsEntityOfType(it->first, EntityType::Hero))
			it = EnemyAbilities.erase(it);
		else ++it;
	}

	EntityList.ForEach<CDOTABaseNPC_Hero>([this](auto hero) {
		if (!CanDraw(hero))
			return;

		if (!EnemyAbilities.count(hero))
			EnemyAbilities[hero].resize(6);
		if (!EnemyItems.count(hero))
			EnemyItems[hero] = {};

		});
}

void Modules::M_AbilityESP::Reset() {
	MTM_LOCK;

	Initialized = false;
	EnemyAbilities.clear();
	EnemyItems.clear();
}

void Modules::M_AbilityESP::UpdateHeroData() {
	MTM_LOCK;

	SubscribeHeroes();
	const auto updateHeroData = [this](CDOTABaseNPC_Hero* hero) {
		DrawableHeroes[hero] =
			IsValidReadPtr(hero) &&
			IsValidReadPtr(hero->GetIdentity()) &&
			!hero->GetIdentity()->IsDormant();

		if (EnemyAbilities.count(hero))
			UpdateAbilities(hero);

		if (EnemyItems.count(hero))
			UpdateItems(hero);
		};

	EntityList.ForEach<CDOTABaseNPC_Hero>(updateHeroData);
	Initialized = true;
}

bool Modules::M_AbilityESP::CanDraw(CDOTABaseNPC_Hero* hero) {
	bool ret = hero
		&& DrawableHeroes[hero]
		&& !hero->IsIllusion()
		&& hero != ctx.localHero
		&& hero->GetLifeState() == 0
		&& IsEntityOnScreen(hero);
	if (!Config::AbilityESP::ShowAllies)
		// I wish they made &&= an operator
		ret = ret && !hero->IsSameTeam(ctx.localHero);
	return ret;
}

void Modules::M_AbilityESP::DrawAbilities() {
	float iconSize = ScaleVar(AbilityIconSize);
	constexpr float outlineThickness = 1;
	constexpr ImVec2 outlineSize{ outlineThickness, outlineThickness };
	constexpr int levelCounterHeight = 8;
	const float rounding = Config::AbilityESP::Rounding / 100.f * iconSize / 2;
	auto DrawList = ImGui::GetForegroundDrawList();
	auto lvlCounterType = (LevelCounterType)Config::AbilityESP::LevelCounterType;

	for (auto& [hero, abilities] : EnemyAbilities) {
		if (!CanDraw(hero))
			continue;
		int abilityCount = 0;
		for (auto& data : abilities)
			if (data.ability)
				++abilityCount;

		auto drawPos = HeroData[hero].W2S;
		drawPos.x -= (abilityCount - 1) * iconSize / 2.0f;
		drawPos.y += 30;

		int idx = 0;
		for (auto& data : abilities) {
			if (!data.ability)
				continue;

			if (!data.icon) {
				data.icon = assets.spellIcons.Load(data.ability->GetIdentity()->GetName());
			}

			// Top-Left, Bottom-Right and Center points of ability icon
			ImVec2 imgXY1, imgXY2, imgCenter;
			float centeringOffset = -outlineThickness + iconSize / 2;
			{
				int idxOffset = idx * iconSize;

				imgXY1 = drawPos - ImVec2{ centeringOffset - idxOffset, centeringOffset };
				imgXY2 = drawPos + ImVec2{ centeringOffset + idxOffset, centeringOffset };
				imgCenter = imgXY1 + ImVec2(centeringOffset, centeringOffset);
			}

			// Black outline by default
			ImU32 outlineColor = ImColor{ 0,0,0 };

			DrawList->AddRectFilled(
				imgXY1 - outlineSize,
				imgXY2 + outlineSize,
				ImColor(0, 0, 0), rounding);


			if (data.ability->Member<bool>(Netvars::C_DOTABaseAbility::m_bAutoCastState))
				outlineColor = ImColor(255, 191, 0);

			if (data.ability->IsToggled())
				outlineColor = ImColor(3, 0xAC, 0x13);

			DrawList->AddRectFilled(
				imgXY1 - outlineSize,
				imgXY2 + outlineSize,
				outlineColor, rounding);

			// Ability icon itself
			DrawList->AddImageRounded(data.icon, imgXY1, imgXY2, { 0,0 }, { 1,1 }, ImColor{ 255,255,255 }, rounding);

			auto getCooldown = [](CDOTABaseAbility* ent)->float {
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
			auto cooldown = getCooldown(data.ability);
			// If the icon must be darker
			bool darkOverlay =
				data.ability->GetLevel() == 0 || // if not learned
				cooldown != 0; // or on cooldown

			if (darkOverlay)
				DrawList->AddRectFilled(imgXY1, imgXY2, ImColor(0, 0, 0, 160), rounding);

			if (cooldown) {
				int cdFontSize = iconSize - ScaleVar(8);
				bool decimals = Config::AbilityESP::ShowCooldownDecimals;
				if (floorf(data.ability->GetCooldown()) >= 100) {
					cdFontSize = iconSize - ScaleVar(12);
					decimals = false;
				}

				if (decimals)
					cdFontSize = iconSize - ScaleVar(14);

				auto textPos = imgCenter - ImVec2{ 0, cdFontSize / 2.f };
				// Draws the cooldown
				DrawTextForeground(DrawData.GetFont("Monofonto", cdFontSize),
					std::vformat(decimals ? "{:.1f}" : "{:.0f}", std::make_format_args(cooldown)),
					textPos,
					cdFontSize,
					ImColor{ 255,255,255 },
					true);
			}
			else if (data.ability->GetManaCost() > hero->GetMana())
				DrawList->AddRectFilled(imgXY1, imgXY2, ImColor(0, 50, 255, 128), rounding);

			if (
				data.ability->GetCharges()
				||
				data.ability->GetCharges() == 0 && data.ability->GetChargeRestoreCooldown() >= 0
				)
				DrawChargeCounter(data.ability->GetCharges(), imgXY1, ScaleVar(8));

			// Channeling countdown
			if (const auto channelTime = data.ability->Member<float>(Netvars::C_DOTABaseAbility::m_flChannelStartTime); channelTime != 0) {
				float indicatorHeight = ScaleVar(4);
				auto channelLength = data.ability->GetLevelSpecialValueFor("AbilityChannelTime");
				int fontSize = ScaleVar(18);

				DrawTextForeground(DrawData.GetFont("Monofonto", fontSize),
					std::format("{:.1f}", channelLength - (GameSystems::GameRules->GetGameTime() - channelTime)),
					ImVec2(imgXY1.x + centeringOffset, imgXY1.y - fontSize - 2 - indicatorHeight),
					fontSize,
					ImVec4(1, 1, 1, 1),
					true);

				float indicatorWidth = abs(imgXY2.x - imgXY1.x) * (1 - ((GameSystems::GameRules->GetGameTime() - channelTime) / channelLength));
				DrawList->AddRectFilled(
					ImVec2(imgXY1.x, imgXY1.y - indicatorHeight),
					ImVec2(imgXY1.x + indicatorWidth, imgXY1.y), ImColor(1, 1, 1, 0.7), rounding);
			}
			// If it's being cast
			else if (data.ability->Member<bool>(Netvars::C_DOTABaseAbility::m_bInAbilityPhase)) {
				float castPoint = data.ability->GetLevelSpecialValueFor("AbilityCastPoint"),
					castStartTime = data.ability->Member<float>(Netvars::C_DOTABaseAbility::m_flCastStartTime);
				int fontSize = ScaleVar(18);
				float indicatorWidthFactor = abs(imgXY1.x - imgXY2.x) * ((GameSystems::GameRules->GetGameTime() - castStartTime) / castPoint);
				DrawList->AddRectFilled(imgXY1, ImVec2(imgXY1.x + indicatorWidthFactor, imgXY2.y), ImColor(0.f, 1.f, 0.f, 0.5f), rounding);
				DrawTextForeground(DrawData.GetFont("Monofonto", fontSize),
					std::format("{:.1f}", castPoint - (GameSystems::GameRules->GetGameTime() - castStartTime)),
					imgXY1 + ImVec2(centeringOffset, -fontSize - 2),
					fontSize,
					ImColor(0, 255, 60),
					true);
			}

			if (lvlCounterType == LevelCounterType::Number
				|| data.ability->GetMaxLevel() > 4) // bars look horrible on Invoker
				DrawLevelCounter(data.ability, ImVec2{ imgXY1.x,imgXY2.y } - ImVec2{ 0, ScaleVar<float>(32) / 6 });
			else
				DrawLevelBars(data.ability,
					ImVec2{ imgXY1.x, imgXY2.y - 3 }, { imgXY2.x, imgXY2.y + 3 });
			++idx;
		}

	}
}

void Modules::M_AbilityESP::LoadItemTexIfNeeded(AbilityData& data) {
	if (data.icon)
		return;

	if (!data.ability->GetIdentity()->GetName())
		return;

	std::string itemName = data.ability->GetIdentity()->GetName();

	if (!data.icon) {
		data.icon = assets.items.Load(itemName.substr(5));
	}

}

// Draws the same block sequence like for abilities + two circles for TP and neutral slot on the right and left respectively
// Only draws slots occupied by an item
// If the item is toggled(like armlet), a green frame is drawn
// If the item has charges(like wand), a counter is displayed in the top left corner of the image
void Modules::M_AbilityESP::DrawItemSequences() {
	const ImVec2 iconSize{ (float)ScaleVar(AbilityIconSize), (float)ScaleVar(AbilityIconSize) };

	const int gap = 1;

	for (auto& [hero, inv] : EnemyItems) {
		if (!CanDraw(hero))
			continue;

		int validItems = 0;
		for (int i = 0; i < 6; i++)
			if (inv[i].ability)
				validItems++;

		if (inv[15].ability)
			validItems++;
		if (inv[16].ability)
			validItems++;

		ImVec2 basePos = HeroData[hero].HealthbarW2S;
		basePos.x -= validItems * (iconSize.x + gap) / 2;
		basePos.y -= 35 + iconSize.x;

		// Adjusting for the debuff indicator that is otherwise obscured by the panel 
		if (hero->Member<uint64_t>(Netvars::C_DOTA_BaseNPC::m_nUnitDebuffState) != 0)
			basePos.y -= 30;

		if (inv[16].ability) {
			LoadItemTexIfNeeded(inv[16]);
			ImVec2 cXY1{ basePos.x + 1 , basePos.y + 1 },
				cXY2 = cXY1 + iconSize - ImVec2{ 2,2 };
			DrawItemCircle(inv[16], cXY1, cXY2, iconSize, (iconSize.x - 2) / 2);
			basePos.x += gap + iconSize.x;
		}

		for (int slot = 0; slot < 6; slot++) {
			if (!inv[slot].ability)
				continue;

			DrawItemIcon(inv, slot, basePos, iconSize);
			basePos.x += gap + iconSize.x;
		}


		if (inv[15].ability) {
			LoadItemTexIfNeeded(inv[15]);
			ImVec2 cXY1{ basePos.x + 1 , basePos.y + 1 },
				cXY2 = cXY1 + iconSize - ImVec2{ 2,2 };
			DrawItemCircle(inv[15], cXY1, cXY2, iconSize, (iconSize.x - 2) / 2);
		}
	}
}

void Modules::M_AbilityESP::DrawItemGrids() {
	const ImVec2 iconSize{ ScaleVar<float>(AbilityIconSize), ScaleVar<float>(AbilityIconSize) };

	const int
		gap = 1,
		col = 3;

	for (auto& [hero, inv] : EnemyItems) {
		if (!CanDraw(hero))
			continue;

		//if (inv[15].ability)
		//	validItems++;
		//if (inv[16].ability)
		//	validItems++;

		ImVec2 basePos = HeroData[hero].HealthbarW2S;
		basePos.x -= col * (iconSize.x + gap) / 2;
		basePos.y -= 10;

		bool sideSlotsPresent = inv[16].ability || inv[15].ability;
		if (sideSlotsPresent)
			basePos.x -= (iconSize.x + gap) / 2 + 4;

		ImVec2 sideSlotsBasePos = basePos;
		sideSlotsBasePos.x += col * (iconSize.x + gap) + 4;

		if (inv[15].ability) {
			LoadItemTexIfNeeded(inv[15]);
			ImVec2 cXY1{ sideSlotsBasePos.x + 1 , sideSlotsBasePos.y + 1 },
				cXY2 = cXY1 + iconSize - ImVec2{ 2,2 };
			DrawItemCircle(inv[15], cXY1, cXY2, iconSize, (iconSize.x - 2) / 2);
		}
		if (inv[16].ability) {
			LoadItemTexIfNeeded(inv[16]);
			ImVec2 cXY1{ sideSlotsBasePos.x + 1 , sideSlotsBasePos.y + iconSize.y + 1 },
				cXY2 = cXY1 + iconSize - ImVec2{ 2,2 };
			DrawItemCircle(inv[16], cXY1, cXY2, iconSize, (iconSize.x - 2) / 2);
		}


		int validItemsDrawn = 0;
		for (int slot = 0; slot < 6; slot++) {
			if (!inv[slot].ability)
				continue;

			int x = validItemsDrawn % col,
				y = validItemsDrawn / col;

			DrawItemIcon(inv, slot, basePos + ImVec2{ (float)x, (float)y } *(iconSize.x + gap), iconSize);

			if (inv[slot].ability)
				validItemsDrawn++;
		}

	}

}

void Modules::M_AbilityESP::DrawItemIcon(std::map<int, AbilityData>& inv, int slot, const ImVec2& pos, const ImVec2& size) {
	auto DrawList = ImGui::GetForegroundDrawList();
	// used to convert native rectangular item images to SQUARES
	constexpr float aspectRatio = (1 - 64. / 88) / 2;
	const float rounding = Config::AbilityESP::Rounding / 100.f * size.x / 2;

	auto& itemData = inv[slot];
	if (!itemData.ability)
		return;

	ImVec2 imgXY1 = pos + ImVec2{ 1,1 },
		imgXY2 = imgXY1 + size - ImVec2{ 2,2 },
		imgCenter = (imgXY1 + imgXY2) / 2;

	ImVec2 frameSize(1, 1);
	ImVec2 frameXY1 = imgXY1 - frameSize,
		frameXY2 = imgXY2 + frameSize;

	ImU32 frameColor = ImColor{ 0,0,0,255 };

	LoadItemTexIfNeeded(itemData);
	DrawList->AddImageRounded(itemData.icon,
		imgXY1,
		imgXY2,
		ImVec2(aspectRatio, 0),
		ImVec2(1 - aspectRatio, 1), ImColor{ 255,255,255 }, rounding);

	if (itemData.ability->IsToggled())
		frameColor = ImColor(0x3, 0xAC, 0x13);


	// Frame
	DrawList->AddRect(frameXY1, frameXY2, frameColor, rounding);

	float cd = itemData.ability->GetCooldown();
	if (cd != 0) {
		DrawList->AddRectFilled(imgXY1, imgXY2, ImColor(0, 0, 0, 130), rounding);
		auto fontSize = size.y - ScaleVar<float>(2);
		bool decimals = Config::AbilityESP::ShowCooldownDecimals;
		if (cd >= 100) {
			fontSize -= 4;
			decimals = false;
		}

		if (decimals)
			fontSize -= 4;
		DrawTextForeground(
			DrawData.GetFont("Monofonto", fontSize),
			std::vformat(Config::AbilityESP::ShowCooldownDecimals ? "{:.1f}" : "{:.0f}", std::make_format_args(cd)),
			ImVec2(imgCenter.x, imgCenter.y - fontSize / 2),
			fontSize,
			ImVec4(1, 1, 1, 1),
			true);
	}

	int charges = reinterpret_cast<CDOTAItem*>(itemData.ability)->GetCurrentCharges();
	if (charges != 0)
		DrawChargeCounter(charges, frameXY1, 8);
}


void Modules::M_AbilityESP::DrawItemCircle(const AbilityData& data, const ImVec2& xy1, const ImVec2& xy2, const ImVec2& iconSize, const int radius) {
	auto DrawList = ImGui::GetForegroundDrawList();
	const ImVec2 center = (xy1 + xy2) / 2;
	constexpr float aspectRatio = (1 - 64.f / 88) / 2;

	DrawList->AddCircleFilled(center, radius + 2, ImColor(0, 0, 0, 255));
	if (!data.ability)
		return;

	DrawList->AddImageRounded(
		data.icon,
		xy1,
		xy2,
		ImVec2(aspectRatio, 0),
		ImVec2(1 - aspectRatio, 1),
		ImGui::GetColorU32(ImVec4(1, 1, 1, 1)),
		radius);

	float cd = data.ability->GetCooldown();
	if (cd == 0)
		return;

	int cdFontSize = radius * 2 - 2;
	if (Config::AbilityESP::ShowCooldownDecimals)
		cdFontSize -= 4;
	// Darkens the picture
	DrawList->AddCircleFilled(center, radius, ImColor(0, 0, 0, 130));
	// Draws the cooldown
	DrawTextForeground(DrawData.GetFont("Monofonto", cdFontSize),
		std::vformat(Config::AbilityESP::ShowCooldownDecimals ? "{:.1f}" : "{:.0f}", std::make_format_args(cd)),
		ImVec2(center.x, center.y - cdFontSize / 2),
		cdFontSize,
		ImVec4(1, 1, 1, 1),
		true);
}

void Modules::M_AbilityESP::DrawESP() {

	if (!Initialized || !Config::AbilityESP::Enabled)
		return;

	MTM_LOCK;

	auto itemType = (ItemPanelType)Config::AbilityESP::ItemPanelType;

	DrawAbilities();
	if (itemType == ItemPanelType::Sequence)
		DrawItemSequences();
	else
		DrawItemGrids();
}

void Modules::M_AbilityESP::DrawLevelCounter(CDOTABaseAbility* ability, const ImVec2& pos) {
	int lvl = ability->GetLevel();
	if (lvl == 0 || lvl == 1 && ability->GetMaxLevel() == 1)
		return;

	// constexpr auto clrLvlOutline = ImVec4(0xE7 / 255.0f, 0xD2 / 255.0f, 0x92 / 255.0f, 1);
	// constexpr auto clrLvlBackground = ImVec4(0x28 / 255.0f, 0x0F / 255.0f, 0x01 / 255.0f, 1);
	// constexpr ImVec2 outlinePadding(1, 1);
	int counterScale = ScaleVar(26);

	// ImVec2 counterSize(counterScale, counterScale);
	// ImVec2 imgXY1 = pos - counterSize, imgXY2 = pos + counterSize;

	auto DrawList = ImGui::GetForegroundDrawList();
	//DrawList->AddRectFilled();
	//DrawRectFilled(imgXY1, counterSize, clrLvlBackground);
	//DrawRect(
	//	imgXY1 + outlinePadding,
	//	counterSize - outlinePadding * 2,
	//	clrLvlOutline
	//);
	DrawTextForeground(DrawData.GetFont("Monofonto", counterScale - 2), std::to_string(lvl),
		ImVec2(pos.x, pos.y - (ScaleVar(32) - 2) / 2),
		counterScale - 2,
		ImColor{ 255,255,255 },
		true);
}

void Modules::M_AbilityESP::DrawLevelBars(CDOTABaseAbility* ability, const ImVec2& xy1, const ImVec2& xy2) {
	const auto clrLearned = ImColor(193, 254, 0);

	int lvl = ability->GetLevel(), maxLvl = ability->GetMaxLevel();
	if (lvl == 1 && maxLvl == 1)
		return;

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

void Modules::M_AbilityESP::DrawChargeCounter(int charges, const ImVec2& pos, int radius) {
	auto DrawList = ImGui::GetForegroundDrawList();

	// Green outline
	// DrawList->AddCircleFilled(pos, radius + 1, ImGui::GetColorU32(ImVec4(135 / 255.0f, 214 / 255.0f, 77 / 255.0f, 1)));
	// Gray core
	// DrawList->AddCircleFilled(pos, radius, ImGui::GetColorU32(ImVec4(0.2, 0.2, 0.2, 1)));
	auto textSize = ScaleVar(20);
	DrawTextForeground(
		DrawData.GetFont("Monofonto", textSize),
		std::to_string(charges),
		ImVec2(pos.x, pos.y - textSize / 2),
		textSize,
		ImVec4(1, 1, 1, 1),
		true);
}

void Modules::M_AbilityESP::UpdateAbilities(CDOTABaseNPC_Hero* hero) {
	auto abilityList = hero->GetAbilities();
	if (abilityList.empty())
		return;

	int validAbilities = 0;
	for (int i = 0; validAbilities != 6; ++i) {
		auto ability = abilityList[i];
		auto& heroAbilities = EnemyAbilities[hero];

		//weird worldent thing
		if (heroAbilities[validAbilities].ability->GetIndex() == 0)
			continue;

		if (heroAbilities[validAbilities].ability == ability) {
			++validAbilities;
			continue;
		}

		// If the ability disappears - like if Aghanim's Scepter is dropped
		if (ability->IsHidden()) {
			heroAbilities[validAbilities] = AbilityData();
			++validAbilities;
			continue;
		}


		auto abilityName = ability->GetIdentity()->GetName();
		if (!abilityName)
			continue;

		heroAbilities[validAbilities] = AbilityData{
			.ability = ability,
			.icon = assets.spellIcons.Load(abilityName)
		};

		++validAbilities;
	}

}

void Modules::M_AbilityESP::UpdateItems(CDOTABaseNPC_Hero* hero) {
	auto heroItems = hero->GetInventory()->GetItems();
	for (int i = 0; i < heroItems.size(); ++i) {
		auto& entry = EnemyItems[hero];
		auto item = Interfaces::EntitySystem->GetEntity<CDOTAItem>(H2IDX(heroItems[i]));
		if (!item) {
			entry.erase(i);
			continue;
		}

		if (entry[i].ability && entry[i].ability->GetIdentity() && entry[i].ability->GetHandle() == heroItems[i])
			continue;

		if (!item->GetIdentity()->GetName())
			continue;

		EnemyItems[hero][i] = AbilityData{
			.ability = item
		};
	}
}
