#include "AbilityESP.h"
#include "../../Input.h"
#include <format>

void ESP::AbilityESP::SubscribeHeroes() {
	// Clean up anything unneeded
	for (auto it = EnemyAbilities.begin(); it != EnemyAbilities.end(); )
	{
		if (!ctx.heroes.count((*it).first))
			it = EnemyAbilities.erase(it);
		else ++it;
	}
	for (auto& hero : ctx.heroes) {
		if (!CanDraw(hero))
			continue;

		if (!EnemyAbilities.count(hero)) {

			EnemyAbilities[hero].reserve(6);
			for (int i = 0; i < 6; ++i)
				EnemyAbilities[hero].push_back(AbilityData());
		}
		if (!EnemyItems.count(hero))
			EnemyItems[hero] = {};
	}
	Initialized = true;
}

void ESP::AbilityESP::Reset() {
	Initialized = false;
	EnemyAbilities.clear();
	EnemyItems.clear();
}

void ESP::AbilityESP::UpdateHeroData() {
	SubscribeHeroes();
	for (auto& hero : ctx.heroes) {
		if (EnemyAbilities.count(hero))
			UpdateAbilities(hero);

		if (EnemyItems.count(hero))
			UpdateItems(hero);
	}
}

bool ESP::AbilityESP::CanDraw(CDOTABaseNPC_Hero* hero) {
	bool ret = hero
		&& !hero->GetIdentity()->IsDormant()
		&& !hero->IsIllusion()
		&& hero != ctx.assignedHero && hero->GetLifeState() == 0;
	if (!Config::AbilityESP::ShowAllies)
		// I wish they made &&= an operator
		ret = ret && hero->GetTeam() != ctx.assignedHero->GetTeam();
	return ret;
}

void ESP::AbilityESP::DrawAbilities(ImFont* textFont) {
	float iconSize = ScaleVar(AbilityIconSize);
	int outlineThickness = 2;
	int manaBarThickness = 18;
	int levelCounterHeight = 8;

	for (auto& [hero, abilities] : EnemyAbilities) {
		if (!CanDraw(hero))
			continue;

		int abilityCount = 0;
		for (auto& data : abilities)
			if (data.ability)
				++abilityCount;

		auto DrawList = ImGui::GetForegroundDrawList();
		auto drawPos = hero->GetPos();

		int x, y;
		Signatures::WorldToScreen(&drawPos, &x, &y, nullptr);
		x -= abilityCount * iconSize / 2.0f;
		y += 30;

		int idx = 0;

		// Background for the whole panel
		DrawRect(
			ImVec2(x - iconSize / 2, y - iconSize / 2),
			ImVec2(abilityCount * iconSize, iconSize + manaBarThickness + levelCounterHeight + 2),
#ifdef _DEBUG
			ImVec4(1, 0, 0, 1)
#else
			ImVec4(0.11f, 0.11f, 0.11f, 1)
#endif // _DEBUG
		);
		float manaPercent = hero->GetMana() / hero->GetMaxMana();

		// Black background to better see the bounds of manabar
		DrawRect(
			ImVec2(x - iconSize / 2 + outlineThickness, y + iconSize / 2 + outlineThickness + levelCounterHeight + 2),
			ImVec2((abilityCount * iconSize - outlineThickness * 2), manaBarThickness - outlineThickness * 2),
			ImVec4(0, 0, 0, 1)
		);
		// Manabar
		DrawRect(
			ImVec2(x - iconSize / 2 + outlineThickness, y + iconSize / 2 + outlineThickness + levelCounterHeight + 2),
			ImVec2((abilityCount * iconSize - outlineThickness * 2) * manaPercent, manaBarThickness - outlineThickness * 2),
			ImVec4(0, 0.4f, 0.9f, 1)
		);
		// Mana info: amount percent% +regen
		DrawTextForeground(
			textFont,
			std::format("{} {}% +{:.1f}", (int)hero->GetMana(), int(manaPercent * 100), hero->GetManaRegen()),
			ImVec2(
				x - iconSize / 2 + outlineThickness + abilityCount * iconSize / 2,
				y + iconSize / 2 + outlineThickness + levelCounterHeight + 2
			),
			manaBarThickness - 4,
			Color(255, 255, 255),
			true
		);

		for (auto& data : abilities) {
			// Top-Left and Bottom-Right points of ability icon
			ImVec2 imgXY1, imgXY2;
			int centeringOffset = -outlineThickness + iconSize / 2;
			{
				int idxOffset = idx * iconSize;
				imgXY1 = { float(x - centeringOffset + idxOffset), float(y - centeringOffset) };
				imgXY2 = { float(x + centeringOffset + idxOffset), float(y + centeringOffset) };
			}

			if (!data.ability)
				continue;
			if (data.ability->Member<bool>(Netvars::C_DOTABaseAbility::m_bAutoCastState))
				DrawList->AddRectFilled(
					ImVec2(imgXY1.x - outlineThickness, imgXY1.y - outlineThickness),
					ImVec2(imgXY2.x + outlineThickness, imgXY2.y + outlineThickness),
					ImGui::GetColorU32(ImVec4(255.0f / 255, 191.0f / 255, 0, 1)));
			if (data.ability->IsToggled())
				DrawList->AddRectFilled(
					ImVec2(imgXY1.x - outlineThickness, imgXY1.y - outlineThickness),
					ImVec2(imgXY2.x + outlineThickness, imgXY2.y + outlineThickness),
					ImGui::GetColorU32(ImVec4(0x3 / 255.0f, 0xAC / 255.0f, 0x13 / 255.0f, 1)));

			DrawList->AddImage(data.icon.glTex, imgXY1, imgXY2);

			if (data.ability->GetLevel() == 0)
				// Darkens the picture
				DrawList->AddRectFilled(imgXY1, imgXY2, ImGui::GetColorU32(ImVec4(0, 0, 0, 0.5)));

			if (
				data.ability->GetCooldown() != 0 || // if on cooldown
				(data.ability->GetCharges() == 0 && // or has 0 charges and a charge cooldown
					data.ability->GetChargeRestoreCooldown() > 0)
				) {
				auto cd = data.ability->GetCooldown() // choosing either of these cooldonws, since they're mutually exclusive
					? data.ability->GetCooldown()
					: data.ability->GetChargeRestoreCooldown();

				int cdFontSize = ScaleVar(14);
				// Darkens the picture
				DrawList->AddRectFilled(imgXY1, imgXY2, ImGui::GetColorU32(ImVec4(0, 0, 0, 0.5)));
				if (data.ability->GetCooldown() >= 100)
					cdFontSize = ScaleVar(12);
				// Draws the cooldown
				DrawTextForeground(textFont,
					std::format("{:.1f}", cd),
					ImVec2(imgXY1.x + centeringOffset, imgXY1.y + (iconSize - cdFontSize) / 2),
					cdFontSize,
					Color(255, 255, 255),
					true);
			}
			DrawChargeCounter(data.ability, textFont, imgXY1, ScaleVar(8));

			float channelTime = data.ability->Member<float>(Netvars::C_DOTABaseAbility::m_flChannelStartTime);
			if (channelTime != 0) {
				float indicatorHeight = ScaleVar(4);
				auto channelLength = data.ability->GetKVValueFor("AbilityChannelTime");
				int fontSize = ScaleVar(18);
				DrawTextForeground(textFont,
					std::format("{:.1f}", channelLength - (GameSystems::GameRules->GetGameTime() - channelTime)),
					ImVec2(imgXY1.x + centeringOffset, imgXY1.y - fontSize - 2 - indicatorHeight),
					fontSize,
					Color(255, 255, 255),
					true);
				float indicatorWidth = abs(imgXY2.x - imgXY1.x) * (1 - ((GameSystems::GameRules->GetGameTime() - channelTime) / channelLength));
				DrawList->AddRectFilled(
					ImVec2(imgXY1.x, imgXY1.y - indicatorHeight),
					ImVec2(imgXY1.x + indicatorWidth, imgXY1.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)));
			}
			// If it's being cast
			else if (data.ability->Member<bool>(Netvars::C_DOTABaseAbility::m_bInAbilityPhase)) {
				auto castPoint = data.ability->GetKVValueFor("AbilityCastPoint");
				float castStartTime = data.ability->Member<float>(Netvars::C_DOTABaseAbility::m_flCastStartTime);
				int fontSize = ScaleVar(18);
				float indicatorWidth = abs(imgXY1.x - imgXY2.x) * ((GameSystems::GameRules->GetGameTime() - castStartTime) / castPoint);
				DrawList->AddRectFilled(imgXY1, ImVec2(imgXY1.x + indicatorWidth, imgXY2.y), ImGui::GetColorU32(ImVec4(0, 1, 0, 0.5)));
				DrawTextForeground(textFont,
					std::format("{:.1f}", castPoint - (GameSystems::GameRules->GetGameTime() - castStartTime)),
					ImVec2(imgXY1.x + centeringOffset, imgXY1.y - fontSize - 2),
					fontSize,
					Color(0, 255, 60),
					true);
			}
			DrawLevelCounter(data.ability, textFont, ImVec2(imgXY1.x + centeringOffset, imgXY2.y + 1));
			++idx;
		}
	}
}

void ESP::AbilityESP::DrawItems(ImFont* textFont) {
	for (auto& [hero, inv] : EnemyItems) {
		if (!CanDraw(hero))
			continue;

		const int row = 3, col = 3;

		int gap =4;
		auto DrawList = ImGui::GetForegroundDrawList();
		auto drawPos = hero->GetPos();
		// Scaled dimensions of item icons, half the size by default
		ImVec2 iconSize = ImVec2(ScaleVar(88 / 2), ScaleVar(64 / 2));
		int x, y;
		Signatures::WorldToScreen(&drawPos, &x, &y, nullptr);
		y += 20;

		float cropAmount = 0.2f;
		auto panelXY1 = ImVec2(x - (iconSize.x + gap) * col / 2.0f, y);
		auto panelSize = ImVec2((iconSize.x + gap) * col + gap, (iconSize.y + gap) * row + gap);
		if (Config::AbilityESP::CropStashItems)
			panelSize.y -= cropAmount * iconSize.y * 2;
		// Background for the whole panel
		DrawRect(
			panelXY1,
			panelSize,
			ImVec4(0.2, 0.2, 0.2, 1.0f)
		);
		for (auto& [slot, itemData] : inv) {
			if (slot > 9)
				continue;
			bool stashSlot = slot > 5;
			int curRow = slot / 3, curCol = slot % 3;
			ImVec2 imgXY1{ panelXY1.x + gap + (iconSize.x + gap) * curCol, panelXY1.y + gap + (gap + iconSize.y) * curRow },
				imgXY2 = ImVec2(imgXY1.x + iconSize.x, imgXY1.y + iconSize.y);

			ImVec2 uvMin{ 0,0 }, uvMax{ 1,1 };
			if (Config::AbilityESP::CropStashItems && stashSlot) {
				uvMin.y += cropAmount;
				uvMax.y -= cropAmount;
				imgXY2.y -= cropAmount * iconSize.y * 2;
			}
			ImVec2 imgCenter = ImVec2((imgXY1.x + imgXY2.x) / 2, (imgXY1.y + imgXY2.y)/2);
			// glTex not checked intentionally so it draws a black slot
			DrawList->AddImage(itemData.icon.glTex, imgXY1, imgXY2, uvMin, uvMax);

			bool darkIcon = false;
			float cd = itemData.ability->GetCooldown();
			if (stashSlot || cd != 0)
				darkIcon = true;
			
			if (darkIcon)
				DrawList->AddRectFilled(imgXY1, imgXY2, ImGui::GetColorU32(ImVec4(0, 0, 0, 0.5f)));
			if (cd)
				DrawTextForeground(
					textFont,
					std::format("{:.1f}", cd),
					ImVec2(imgCenter.x, imgCenter.y - ScaleVar(12/2)),
					ScaleVar<float>(12),
					Color(255, 255, 255),
					true);
		}

	}
}

void ESP::AbilityESP::DrawESP(ImFont* textFont) {
	if (!Initialized || !Config::AbilityESP::Enabled)
		return;

	for (auto& [path, data] : loadingQueue)
		LoadTexture(path.c_str(), *data);


	loadingQueue.clear();

	// ALT key
	if (!GetAsyncKeyState(164))
		DrawAbilities(textFont);
	else
		DrawItems(textFont);
}

void ESP::AbilityESP::DrawLevelCounter(CDOTABaseAbility* ability, ImFont* font, ImVec2 pos) {
	int lvl = ability->GetLevel();
	DrawTextForeground(font, std::format("LVL {}", lvl), pos, 12, Color(255, 255, 255, 255), true, false);
}

void ESP::AbilityESP::DrawChargeCounter(CDOTABaseAbility* ability, ImFont* font, ImVec2 pos, int radius) {
	if (ability->GetCharges() == 0 &&
		ability->GetChargeRestoreCooldown() <= 0)
		return;
	auto DrawList = ImGui::GetForegroundDrawList();

	// Green outline
	DrawList->AddCircleFilled(pos, radius + ScaleVar(2), ImGui::GetColorU32(ImVec4(135 / 255.0f, 214 / 255.0f, 77 / 255.0f, 1)));
	// Gray core
	DrawList->AddCircleFilled(pos, radius, ImGui::GetColorU32(ImVec4(0.2, 0.2, 0.2, 1)));

	DrawTextForeground(
		font,
		std::to_string(ability->GetCharges()),
		ImVec2(pos.x, pos.y - ScaleVar(6)),
		ScaleVar(12),
		Color(255, 255, 255, 255),
		true);
}

void ESP::AbilityESP::UpdateAbilities(CDOTABaseNPC_Hero* hero) {
	auto heroAbilities = hero->GetAbilities();
	if (heroAbilities.empty())
		return;

	for (int i = 0; i < 6; ++i) {
		auto ability = heroAbilities[i];
		auto& heroAbilities = EnemyAbilities[hero];

		if (heroAbilities[i].ability == ability)
			continue;

		// If the ability disappears - like if Aghanim's Scepter is dropped
		if (ability->IsHidden()) {
			heroAbilities[i] = AbilityData();
			continue;
		}


		auto abilityName = ability->GetIdentity()->GetName();
		if (!abilityName)
			return;
		auto iconPath = ctx.cheatFolderPath + "\\assets\\spellicons\\" + abilityName + "_png.png";
		auto& data = heroAbilities[i] = AbilityData{
			.ability = ability,
			.lastActiveTime = GameSystems::GameRules->GetGameTime(),
			.lastActiveCooldown = ability->GetCooldown(),
			.currentCooldown = ability->GetCooldown()
		};
		loadingQueue[iconPath] = &data.icon;
	}

}

void ESP::AbilityESP::UpdateItems(CDOTABaseNPC_Hero* hero) {
	auto heroItems = hero->GetInventory()->GetItems();
	for (int i = 0; i < heroItems.size(); ++i) {
		auto& entry = EnemyItems[hero];
		if (!HVALID(heroItems[i])) {
			entry.erase(i);
			continue;
		}

		if (entry[i].ability && entry[i].ability->GetIdentity() && entry[i].ability->GetHandle() == heroItems[i])
			continue;

		auto item = Interfaces::EntitySystem->GetEntity<CDOTAItem>(H2IDX(heroItems[i]));
		if (!item->GetIdentity()->GetName())
			continue;

		// Image name doesn't use the "item_" prefix
		std::string itemName = item->GetIdentity()->GetName();
		auto iconPath = ctx.cheatFolderPath + "\\assets\\items\\" + itemName.substr(5) + "_png.png";
		auto data = AbilityData{
			.ability = item,
			.lastActiveTime = GameSystems::GameRules->GetGameTime(),
			.lastActiveCooldown = item->GetCooldown(),
			.currentCooldown = item->GetCooldown()
		};
		entry[i] = data;
		loadingQueue[iconPath] = &entry[i].icon;
	}
}
