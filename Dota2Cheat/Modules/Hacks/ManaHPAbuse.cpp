#include "ManaHPAbuse.h"
#include "../../Hooks/PrepareUnitOrders.h"

void Modules::M_ManaHPAbuse::ReorderItems(const std::map<CDOTAItem*, int>& positions) {
	for (auto& [item, pos] : positions)
		ctx.localPlayer->PrepareOrder(
			Order()
			.SetType(DOTA_UNIT_ORDER_MOVE_ITEM)
			.SetAbilityIndex(item->GetIndex())
			.SetTargetIndex(pos)
			.SetQueue(true)
			.SetIssuer(ctx.localHero));
}

void Modules::M_ManaHPAbuse::DetermineBonusType(CDOTAItem* item) {
	switch (abusedItems[item->GetIdentity()->GetName()]) {
	case Health: {
		static std::vector<const char*> healthTypes = {
			"bonus_str",
			"bonus_strength",
			"bonus_all_stats",
			"bonus_health"
		};
		bonusTypes = &healthTypes;
		break;
	}
	case Mana: {
		static std::vector<const char*> manaTypes = {
			"bonus_int",
			"bonus_intellect",
			"bonus_all_stats",
			"bonus_mana",
		};
		bonusTypes = &manaTypes;
		break;
	}
	case Both: {
		static std::vector<const char*> bothTypes = {
			"bonus_int",
			"bonus_str",
			"bonus_intellect",
			"bonus_strength",
			"bonus_all_stats",
			"bonus_mana",
			"bonus_health"
		};
		bonusTypes = &bothTypes;
		break;
	};
	}
}

bool Modules::M_ManaHPAbuse::ItemNeedsExclusion(CDOTAItem* item) {
	if (!bonusTypes)
		return false;

	for (auto bonus : *bonusTypes)
		if (item->GetLevelSpecialValueFor(bonus) > 0)
			return true;
	return false;
}

void Modules::M_ManaHPAbuse::ChangeItemStatTo(CDOTAItem* item, ItemStat stat, CBaseEntity* issuer) {
	if (!item || item->GetItemStat() == stat)
		return;
	int diff = (int)stat - (int)item->GetItemStat();
	int cycles = diff > 0 ? diff : diff + 3;

	for (int i = 0; i < cycles; i++)
		ctx.localPlayer->PrepareOrder(
			Order()
			.SetType(DOTA_UNIT_ORDER_CAST_NO_TARGET)
			.SetAbilityIndex(item->GetIndex())
			.SetIssuer(issuer)
			.SetQueue(true)

		);
}

void Modules::M_ManaHPAbuse::GetItemsForExclusion(CDOTABaseNPC* npc, std::set<CDOTAItem*> preservedItems, std::map<CDOTAItem*, int>& cachedPositions, std::stack<CDOTAItem*>& itemsToExclude) {

	auto items = npc->GetInventory()->GetItems();
	for (int i = 0; i < 6; i++) {
		auto item = items[i];
		if (item.IsValid() && !preservedItems.contains(item) && ItemNeedsExclusion(item)) {
			cachedPositions[item] = i;
			itemsToExclude.push(item);
		}
	}

	// Neutral slot
	if (items[16].IsValid() && !preservedItems.contains(items[16]) && ItemNeedsExclusion(items[16])) {
		cachedPositions[items[16]] = 16;
		itemsToExclude.push(items[16]);
	}

}

void Modules::M_ManaHPAbuse::DropMode(CDOTABaseNPC* npc, CDOTAItem* ability, std::map<CDOTAItem*, int>& cachedPositions, std::stack<CDOTAItem*>& itemsToExclude) {
	if (itemsToExclude.size() == 0)
		return;

	// Wouldn't want to drop items near enemies
	bool enemiesAround = EntityList.ContainsTypes([npc](const auto& wrap) {
		auto hero = wrap.As<CDOTABaseNPC_Hero>();
	return hero != npc
		&& !hero->IsSameTeam(npc)
		&& hero->IsTargetable()
		&& IsWithinRadius(hero->GetPos(),
			npc->GetPos(),
			Config::ManaAbuse::SafetyRadius);
		}, EntityType::Hero);

	if (enemiesAround)
		return;

	while (itemsToExclude.size() != 0) {
		auto item = itemsToExclude.top();
		ctx.localPlayer->PrepareOrder(
			Order()
			.SetType(DOTA_UNIT_ORDER_DROP_ITEM)
			.SetAbilityIndex(item->GetIndex())
			.SetPosition(npc->GetPos())
			.SetQueue(true)
			.SetIssuer(npc));

		itemsToExclude.pop();
	}

}

void Modules::M_ManaHPAbuse::MoveMode(CDOTABaseNPC* npc, CDOTAItem* ability, std::map<CDOTAItem*, int>& cachedPositions, std::stack<CDOTAItem*>& itemsToExclude) {
	while (itemsToExclude.size() != 0) {
		auto items = npc->GetInventory()->GetItems();
		auto item = itemsToExclude.top();
		int slot = cachedPositions[item];
		int stashSlot = 6;
		if (slot == 15) {
			stashSlot = -1;
			for (int i = 6; i < 9; i++)
				if (!items[i].IsValid())
					stashSlot = i;
		}
		if (stashSlot == -1) {
			itemsToExclude.pop();
			continue;
		}

		ctx.localPlayer->PrepareOrder(
			Order()
			.SetType(DOTA_UNIT_ORDER_MOVE_ITEM)
			.SetAbilityIndex(item->GetIndex())
			.SetTargetIndex(stashSlot)
			.SetQueue(true)
			.SetIssuer(npc));

		ctx.localPlayer->PrepareOrder(
			Order()
			.SetType(DOTA_UNIT_ORDER_MOVE_ITEM)
			.SetAbilityIndex(item->GetIndex())
			.SetTargetIndex(slot)
			.SetQueue(true)
			.SetIssuer(npc));

		itemsToExclude.pop();
	}
}

void Modules::M_ManaHPAbuse::PerformAbuse(CDOTABaseNPC* npc, CDOTAItem* ability) {
	if (!Config::ManaAbuse::Enabled)
		return;

	if (!abusedItems.contains(
		ability
		->GetIdentity()
		->GetName()))
		return;

	DetermineBonusType(ability);

	std::map<CDOTAItem*, ItemStat> origItemStats;

	// Items that cannot be excluded
	std::set<CDOTAItem*> preservedItems{
		ability
	};

	// Switches an item's stat if it doesn't already have the required one
	// Records the original stat so it can be restored later
	auto adjustItemStat = [this, &origItemStats, &preservedItems, ability, npc](const char* itemName) mutable {
		auto bonusType = abusedItems[ability->GetIdentity()->GetName()];
		if (auto statItem = HeroData[ctx.localHero].Items[itemName]) {
			if (bonusType == Mana && statItem->GetItemStat() == ItemStat::INTELLIGENCE
				|| bonusType == Health && statItem->GetItemStat() == ItemStat::STRENGTH
				|| bonusType == Both && statItem->GetItemStat() != ItemStat::AGILITY) {
				origItemStats[statItem] = statItem->GetItemStat();
				ChangeItemStatTo(statItem, ItemStat::AGILITY, npc);
				preservedItems.insert(statItem);
			}
		}
	};

	adjustItemStat("power_treads");
	adjustItemStat("vambrace");

	std::stack<CDOTAItem*> itemsToExclude;
	std::map<CDOTAItem*, int> itemPositions;

	GetItemsForExclusion(npc, preservedItems, itemPositions, itemsToExclude);

	if (
		(Mode)Config::ManaAbuse::Mode == Mode::Move
		// Move mode relies on 6-second stash cooldowns, which shops negate
		&& npc->Member<DOTA_SHOP_TYPE>(Netvars::C_DOTA_BaseNPC::m_iCurShop) == DOTA_SHOP_NONE
		)
		MoveMode(npc, ability, itemPositions, itemsToExclude);
	else {
		isInterruptible = false;
		DropMode(npc, ability, itemPositions, itemsToExclude);
	}

	auto revert = [&, this, origItemStats, npc, itemPositions]() {
		Sleep(300);
		for (auto& [item, stat] : origItemStats)
			ChangeItemStatTo(item, stat, npc);

		if (
			(Mode)Config::ManaAbuse::Mode == Mode::Drop
			|| npc->Member<uint32_t>(Netvars::C_DOTA_BaseNPC::m_iCurShop) != 8
			) {
			EntityList.ForEachOfType(EntityType::PhysicalItem, [this, npc](auto& item) {
				if (IsWithinRadius(item->GetPos(), ctx.localHero->GetPos(), 150))
				ctx.localPlayer->PrepareOrder(
					Order()
					.SetType(DOTA_UNIT_ORDER_PICKUP_ITEM)
					.SetTargetIndex(item->GetIndex())
					.SetQueue(true)
					.SetIssuer(npc)
				);
				});
		}
		ReorderItems(itemPositions);

		isInterruptible = true;
	};

	// Multithreading magic — who knows when the hero finishes dropping the items?
	manaAbuseReverse = std::async(std::launch::async, revert);

}

