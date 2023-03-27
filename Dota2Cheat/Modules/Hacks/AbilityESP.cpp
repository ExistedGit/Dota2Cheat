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
		DrawableHeroes[hero] = hero && hero->GetIdentity() && !hero->GetIdentity()->IsDormant();
		if (EnemyAbilities.count(hero))
			UpdateAbilities(hero);

		if (EnemyItems.count(hero))
			UpdateItems(hero);
	}
}

bool ESP::AbilityESP::CanDraw(CDOTABaseNPC_Hero* hero) {
	bool ret = hero
		&& DrawableHeroes[hero]
		&& !hero->IsIllusion()
		&& hero != ctx.assignedHero && hero->GetLifeState() == 0;
	if (!Config::AbilityESP::ShowAllies)
		// I wish they made &&= an operator
		ret = ret && hero->GetTeam() != ctx.assignedHero->GetTeam();
	return ret;
}

void ESP::AbilityESP::DrawAbilities() {
	float iconSize = ScaleVar(AbilityIconSize);
	constexpr int outlineThickness = 2;
	constexpr int levelCounterHeight = 8;
	auto DrawList = ImGui::GetForegroundDrawList();

	for (auto& [hero, abilities] : EnemyAbilities) {
		if (!CanDraw(hero))
			continue;

		int abilityCount = 0;
		for (auto& data : abilities)
			if (data.ability)
				++abilityCount;

		auto drawPos = hero->GetPos();

		drawPos.x = (int)(drawPos.x * 100) / 100.0f;
		drawPos.y = (int)(drawPos.y * 100) / 100.0f;

		int x, y;
		Signatures::WorldToScreen(&drawPos, &x, &y, nullptr);
		x -= abilityCount * iconSize / 2.0f;
		y += 30;

		int idx = 0;

		// Background for the whole panel
//		DrawRectFilled(
//			ImVec2(x - iconSize / 2, y - iconSize / 2),
//			ImVec2(abilityCount * iconSize, iconSize + levelCounterHeight + 2),
//#ifdef _DEBUG
//			ImVec4(1, 0, 0, 1)
//#else
//			ImVec4(0.11f, 0.11f, 0.11f, 1)
//#endif // _DEBUG
//		);
		// Mana info: amount percent% +regen
		//DrawTextForeground(
		//	textFont,
		//	std::format("{} {}% +{:.1f}", (int)hero->GetMana(), int(manaPercent * 100), hero->GetManaRegen()),
		//	ImVec2(
		//		x - iconSize / 2 + outlineThickness + abilityCount * iconSize / 2,
		//		y + iconSize / 2 + outlineThickness + levelCounterHeight + 2
		//	),
		//	manaBarThickness - 4,
		//	Color(255, 255, 255),
		//	true
		//);

		for (auto& data : abilities) {
			if (!data.ability)
				continue;

			if (data.icon.glTex == 0) {
				auto tex = texManager.GetNamedTexture(data.ability->GetIdentity()->GetName());
				if (tex)
					data.icon = *tex;
			}

			// Top-Left and Bottom-Right points of ability icon
			ImVec2 imgXY1, imgXY2;
			int centeringOffset = -outlineThickness + iconSize / 2;
			{
				int idxOffset = idx * iconSize;
				imgXY1 = { float(x - centeringOffset + idxOffset), float(y - centeringOffset) };
				imgXY2 = { float(x + centeringOffset + idxOffset), float(y + centeringOffset) };
			}

			DrawList->AddRectFilled(
				ImVec2(imgXY1.x - outlineThickness / 2, imgXY1.y - outlineThickness / 2),
				ImVec2(imgXY2.x + outlineThickness / 2, imgXY2.y + outlineThickness / 2),
				ImGui::GetColorU32(ImVec4(0, 0, 0, 1)));

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
				auto cd = data.ability->GetCooldown() // choosing either of these cooldowns, since they're mutually exclusive
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
					ImVec4(1, 1, 1, 1),
					true);
			}
			if (data.ability->GetCharges() ||
				(data.ability->GetCharges() == 0 &&
					data.ability->GetChargeRestoreCooldown() >= 0))
				DrawChargeCounter(data.ability->GetCharges(), imgXY1, ScaleVar(8));

			float channelTime = data.ability->Member<float>(Netvars::C_DOTABaseAbility::m_flChannelStartTime);
			if (channelTime != 0) {
				float indicatorHeight = ScaleVar(4);
				auto channelLength = data.ability->GetKVValueFor("AbilityChannelTime");
				int fontSize = ScaleVar(18);
				DrawTextForeground(textFont,
					std::format("{:.1f}", channelLength - (GameSystems::GameRules->GetGameTime() - channelTime)),
					ImVec2(imgXY1.x + centeringOffset, imgXY1.y - fontSize - 2 - indicatorHeight),
					fontSize,
					ImVec4(1, 1, 1, 1),
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
					ImVec4(0, 1, 60 / 255.0f, 1),
					true);
			}
			DrawLevelCounter(data.ability, ImVec2(imgXY2.x - centeringOffset, imgXY2.y));
			++idx;
		}
	}
}

void ESP::AbilityESP::LoadItemTexIfNeeded(AbilityData& data) {
	if (data.icon.glTex == 0) {
		std::string itemName = data.ability->GetIdentity()->GetName();
		auto tex = texManager.GetNamedTexture(itemName.substr(5));
		if (tex)
			data.icon = *tex;
	}
}

// Draws a 3x3 grid of items
// If there is no item in a slot, a black block is drawn
// If the item is toggled(like armlet), a green frame is drawn
// If the item has charges(like wand), a circle with a counter is drawn in the top left corner of the image
void ESP::AbilityESP::DrawItems() {
	for (auto& [hero, inv] : EnemyItems) {
		if (!CanDraw(hero))
			continue;

		const int rows = 3, cols = 3;

		int gap = 4;
		// Scaled dimensions of item icons, half the size by default
		ImVec2 iconSize = ImVec2(ScaleVar(88 / 2), ScaleVar(64 / 2));
		auto DrawList = ImGui::GetForegroundDrawList();
		auto drawPos = hero->GetPos();

		// here the rounding isn't as critical for some reason
		//drawPos.x = (int)(drawPos.x * 100) / 100.0f;
		//drawPos.y = (int)(drawPos.y * 100) / 100.0f;

		int x, y;
		Signatures::WorldToScreen(&drawPos, &x, &y, nullptr);
		y += 20;

		float cropAmount = 0.2f;
		auto panelXY1 = ImVec2(x - (iconSize.x + gap) * cols / 2.0f, y);
		auto panelSize = ImVec2((iconSize.x + gap) * cols + gap, (iconSize.y + gap) * rows + gap);

		int circleRadius = panelSize.y / 6;

		if (Config::AbilityESP::CropStashItems)
			panelSize.y -= cropAmount * iconSize.y * 2;
		// Background for the whole panel
		DrawRectFilled(
			panelXY1,
			panelSize,
			ImVec4(0.2, 0.2, 0.2, 1.0f)
		);
		for (int slot = 0; slot < 9; slot++) {
			bool stashSlot = slot > 5;
			int row = slot / 3, col = slot % 3;
			ImVec2 imgXY1{ panelXY1.x + gap + (iconSize.x + gap) * col, panelXY1.y + gap + (gap + iconSize.y) * row },
				imgXY2 = ImVec2(imgXY1.x + iconSize.x, imgXY1.y + iconSize.y);

			ImVec2 uvMin{ 0,0 }, uvMax{ 1,1 };
			if (Config::AbilityESP::CropStashItems && stashSlot) {
				uvMin.y += cropAmount;
				uvMax.y -= cropAmount;
				imgXY2.y -= cropAmount * iconSize.y * 2;
			}
			ImVec2 imgCenter = ImVec2((imgXY1.x + imgXY2.x) / 2, (imgXY1.y + imgXY2.y) / 2);

			int frameThickness = gap / 2;
			ImVec2 frameXY1{ imgXY1.x - frameThickness, imgXY1.y - frameThickness },
				frameXY2{ imgXY2.x + frameThickness, imgXY2.y + frameThickness };
			bool drawFrame = false;
			ImU32 frameColor = 0;

			if (!inv.count(slot) || !inv[slot].ability) {
				DrawList->AddRectFilled(imgXY1, imgXY2, ImGui::GetColorU32({ 0, 0, 0, 1 }));
				continue;
			}

			auto& itemData = inv[slot];
			LoadItemTexIfNeeded(itemData);
			DrawList->AddImage(itemData.icon.glTex, imgXY1, imgXY2, uvMin, uvMax);

			if (itemData.ability->IsToggled()) {
				drawFrame = true;
				frameColor = ImGui::GetColorU32(ImVec4(0x3 / 255.0f, 0xAC / 255.0f, 0x13 / 255.0f, 1));
			}

			// Frame
			if (drawFrame)
				DrawList->AddRect(frameXY1, frameXY2, frameColor);

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
					ImVec2(imgCenter.x, imgCenter.y - ScaleVar(14 / 2)),
					ScaleVar<float>(14),
					ImVec4(1, 1, 1, 1),
					true);

			int charges = reinterpret_cast<CDOTAItem*>(itemData.ability)->GetCurrentCharges();
			if (charges != 0)
				DrawChargeCounter(charges, imgXY1, 8);
		}

		float circleX = panelXY1.x + panelSize.x + circleRadius + gap;
		DrawList->AddCircleFilled(ImVec2(circleX, panelXY1.y + panelSize.y / 4), circleRadius, ImGui::GetColorU32(ImVec4(0.2, 0.2, 0.2, 1.0f)));
		DrawList->AddCircleFilled(ImVec2(circleX, panelXY1.y + panelSize.y / 4 * 3), circleRadius, ImGui::GetColorU32(ImVec4(0.2, 0.2, 0.2, 1.0f)));
		if (inv.count(16) && inv[16].ability) {
			LoadItemTexIfNeeded(inv[16]);
			ImVec2 cXY1{ circleX - iconSize.y / 2, panelXY1.y + panelSize.y / 4 - iconSize.y / 2 },
				cXY2{ cXY1.x + iconSize.y, cXY1.y + iconSize.y };
			DrawItemCircle(inv[16], cXY1, cXY2, iconSize, circleRadius);
		}
		if (inv.count(15) && inv[15].ability) {
			LoadItemTexIfNeeded(inv[15]);
			ImVec2 cXY1{ circleX - iconSize.y / 2, panelXY1.y + panelSize.y / 4 * 3 - iconSize.y / 2 },
				cXY2{ cXY1.x + iconSize.y, cXY1.y + iconSize.y };
			DrawItemCircle(inv[15], cXY1, cXY2, iconSize, circleRadius);
		}

	}
}

void ESP::AbilityESP::DrawItemCircle(const AbilityData& data, const ImVec2& xy1, const ImVec2& xy2, const ImVec2& iconSize, const int radius) {
	auto DrawList = ImGui::GetForegroundDrawList();
	float ratio = (1 - iconSize.y / iconSize.x) / 2;
	DrawList->AddImageRounded(
		data.icon.glTex,
		xy1,
		xy2,
		ImVec2(ratio, 0),
		ImVec2(1 - ratio, 1),
		ImGui::GetColorU32(ImVec4(1, 1, 1, 1)),
		radius);
	float cd = data.ability->GetCooldown();
	if (cd) {
		ImVec2 center((xy1.x + xy2.x) / 2, (xy1.y + xy2.y) / 2);
		int cdFontSize = ScaleVar(14);
		// Darkens the picture
		DrawList->AddCircleFilled(center, radius, ImGui::GetColorU32(ImVec4(0, 0, 0, 0.5)));
		// Draws the cooldown
		DrawTextForeground(textFont,
			std::format("{:.1f}", cd),
			ImVec2(center.x, center.y - cdFontSize / 2),
			cdFontSize,
			ImVec4(1, 1, 1, 1),
			true);
	}

}

void ESP::AbilityESP::DrawESP() {
	if (!textFont)
		return;
	if (!Initialized || !Config::AbilityESP::Enabled)
		return;
	DrawManabars();
	// ALT key
	if (!GetAsyncKeyState(164))
		DrawAbilities();
	else
		DrawItems();
}

void ESP::AbilityESP::DrawLevelCounter(CDOTABaseAbility* ability, ImVec2 pos) {
	int lvl = ability->GetLevel();
	if (lvl == 0)
		return;

	constexpr static auto clrLvlOutline = ImVec4(0xE7 / 255.0f, 0xD2 / 255.0f, 0x92 / 255.0f, 1);
	constexpr static auto clrLvlBackGround = ImVec4(0x28 / 255.0f, 0x0F / 255.0f, 0x01 / 255.0f, 1);
	constexpr static int counterSize = 16;
	constexpr static int outlinePadding = 1;

	ImVec2 imgXY1{ pos.x - counterSize / 2, pos.y - counterSize / 2 };
	DrawRectFilled(imgXY1, ImVec2(counterSize, counterSize), clrLvlBackGround);
	DrawRect(
		ImVec2(imgXY1.x + outlinePadding, imgXY1.y + outlinePadding),
		ImVec2(counterSize - outlinePadding * 2, counterSize - outlinePadding * 2),
		clrLvlOutline
	);
	DrawTextForeground(textFont, std::to_string(lvl), ImVec2(pos.x, pos.y - (counterSize - 4) / 2), counterSize - 4, clrLvlOutline, true, false);
}

void ESP::AbilityESP::DrawChargeCounter(int charges, ImVec2 pos, int radius) {
	auto DrawList = ImGui::GetForegroundDrawList();

	// Green outline
	DrawList->AddCircleFilled(pos, radius + ScaleVar(2), ImGui::GetColorU32(ImVec4(135 / 255.0f, 214 / 255.0f, 77 / 255.0f, 1)));
	// Gray core
	DrawList->AddCircleFilled(pos, radius, ImGui::GetColorU32(ImVec4(0.2, 0.2, 0.2, 1)));

	DrawTextForeground(
		textFont,
		std::to_string(charges),
		ImVec2(pos.x, pos.y - (radius - 2)),
		(radius - 2) * 2,
		ImVec4(1, 1, 1, 1),
		true,
		false);
}

void ESP::AbilityESP::DrawManabars() {
	constexpr static ImVec2 manabarSize{ 104, 8 };
	for (auto& hero : ctx.heroes) {
		if (hero->GetTeam() == ctx.assignedHero->GetTeam())
			continue;
		int hbo = hero->Member<int>(Netvars::C_DOTA_BaseNPC::m_iHealthBarOffset);

		Vector pos = hero->IsMoving()
			? pos = hero->GetForwardVector(hero->GetMoveSpeed() * (-0.0167f)) // Going back 1 frame to synchronize with the game
			: pos = hero->GetPos();
		pos.z += hbo;
		int dx = 0, dy = 0;
		Signatures::WorldToScreen(&pos, &dx, &dy, nullptr);
		dy -= 14;
		dx += 2;
		DrawRectFilled(
			ImVec2(dx - 110 / 2, dy - manabarSize.y / 2),
			manabarSize, ImVec4(0, 0, 0, 1));
		DrawRectFilled(
			ImVec2(dx - 110 / 2 + 1, dy - manabarSize.y / 2 + 1),
			ImVec2(manabarSize.x * (hero->GetMana() / hero->GetMaxMana()) - 2, manabarSize.y - 2), ImVec4(0, 0.5, 1, 1));
	}
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
		texManager.QueueForLoading(iconPath, abilityName);
	}

}

void ESP::AbilityESP::UpdateItems(CDOTABaseNPC_Hero* hero) {
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

		// Image name doesn't use the "item_" prefix
		std::string itemName = item->GetIdentity()->GetName();
		auto iconPath = ctx.cheatFolderPath + "\\assets\\items\\" + itemName.substr(5) + "_png.png";
		EnemyItems[hero][i] = AbilityData{
			.ability = item,
			.lastActiveTime = GameSystems::GameRules->GetGameTime(),
			.lastActiveCooldown = item->GetCooldown(),
			.currentCooldown = item->GetCooldown()
		};
		texManager.QueueForLoading(iconPath, itemName.substr(5));
	}
}
