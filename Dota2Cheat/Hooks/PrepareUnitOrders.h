#pragma once
#include <future>
#include "../SDK/Globals/Signatures.h"
#include "../Modules/Hacks/BadCastPrevention.h"
#include "../Modules/Hacks/PerfectBlink.h"
#include "../Modules/Hacks/CastRedirection.h"
#include "../SDK/Base/StringUtils.h"

namespace Hooks {
	inline Signatures::PrepareUnitOrdersFn oPrepareUnitOrders = nullptr;

	inline std::future<void> manaAbusePickup;

	inline void ChangeItemStatTo(CDOTAItem* item, ItemStat_t stat, CDOTAPlayerController* player, CBaseEntity* issuer) {
		if (!item || item->GetItemStat() == stat)
			return;
		int diff = (int)stat - (int)item->GetItemStat();
		int cycles = diff > 0 ? diff : diff + 3;

		for (int i = 0; i < cycles; i++) {
			oPrepareUnitOrders(
				player,
				DOTA_UNIT_ORDER_CAST_NO_TARGET,
				0,
				&Vector::Zero,
				item->GetIndex(),
				DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
				issuer,
				true,
				false);
		}

	}

	inline void hkPrepareUnitOrders(CDOTAPlayerController* player, dotaunitorder_t orderType, UINT32 targetIndex, Vector* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue, bool showEffects) {
		//std::cout << "[ORDER] " << player << '\n';
		bool giveOrder = true; // whether or not the function will continue

		if (!issuer) { // issuer may be nullptr if it's HERO_ONLY or something
			switch (orderIssuer) {
			case DOTA_ORDER_ISSUER_HERO_ONLY:
				issuer = ctx.assignedHero;
				break;
			case DOTA_ORDER_ISSUER_CURRENT_UNIT_ONLY:
			case DOTA_ORDER_ISSUER_SELECTED_UNITS:
				if (!ctx.localPlayer->GetSelectedUnits().empty())
					issuer = Interfaces::EntitySystem->GetEntity(ctx.localPlayer->GetSelectedUnits().front());
				break;
			}
		}
		if (issuer)
			switch (orderType) {
			case DOTA_UNIT_ORDER_CAST_TARGET:
			{
				Modules::CastRedirection.RedirectIfIllusionCast(targetIndex, issuer, abilityIndex, giveOrder);
				break;
			}
			case DOTA_UNIT_ORDER_CAST_POSITION: {
				Modules::PerfectBlink.AdjustIfBlink(position, abilityIndex, issuer);
				break;
			}
			case DOTA_UNIT_ORDER_DROP_ITEM:
			{
				break;
			}
			case DOTA_UNIT_ORDER_CAST_NO_TARGET: {
				//Automatic mana & HP abuse with items like Arcane Boots or Faerie Fire
				static std::vector<const char*> filters = {
					"item_arcane_boots", "item_enchanted_mango",
					"item_guardian_greaves",
					"item_magic_stick",
					"item_magic_wand",
					"item_holy_locket",
					"item_soul_ring", "item_cheese", "item_arcane_ring", "item_faerie_fire", "item_greater_faerie_fire"
				};
				static std::vector<const char*> bonusTypes = {
					"bonus_int",
					"bonus_intellect",
					"bonus_strength",
					"bonus_str",
					"bonus_all_stats",
					"bonus_mana",
					"bonus_health"
				};
				if (issuer == nullptr)
					issuer = player->GetAssignedHero();

				if (!TestStringFilters(
					Interfaces::EntitySystem
					->GetEntity<CDOTABaseAbility>(abilityIndex)
					->GetIdentity()
					->GetName(),
					filters))
					break;

				CDOTABaseNPC* npc = (CDOTABaseNPC*)issuer;
				bool callPickup = false;

				std::map<CDOTAItem*, ItemStat_t> origItemStats{
				};
				if (ctx.importantItems.power_treads) {

					origItemStats[ctx.importantItems.power_treads] = ctx.importantItems.power_treads->GetItemStat();
					ChangeItemStatTo(ctx.importantItems.power_treads, ItemStat_t::AGILITY, player, issuer);
					callPickup = true;
				}
				if (ctx.importantItems.vambrace) {

					origItemStats[ctx.importantItems.vambrace] = ctx.importantItems.vambrace->GetItemStat();
					ChangeItemStatTo(ctx.importantItems.vambrace, ItemStat_t::AGILITY, player, issuer);
					callPickup = true;
				}

				auto items = npc->GetItems();
				for (auto& item : items) {
					auto itemSlot = npc->GetInventory()->GetItemSlot(item->GetHandle());
					if (
						item->GetIndex() == abilityIndex                   // must not be the item we're using
						||
						(
							itemSlot > 5 && // must not be in the backpack
							itemSlot != 15 // but can be in the neutral slot
							)
						)
						continue;

					double anyBonus = 0;
					for (auto& bonus : bonusTypes) {
						anyBonus = item->GetLevelSpecialValueFor(bonus, -1);
						if (anyBonus > 0)
							break;
					}
					auto fVec = npc->GetForwardVector(5);
					if (anyBonus > 0) {
						//std::cout << abilityIndex << bonusInt << '\n';
						queue = true;
						oPrepareUnitOrders(player, DOTA_UNIT_ORDER_DROP_ITEM, 0, &fVec, item->GetIndex(), DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, issuer, true, false);
						callPickup = true;
					}
				}
				if (callPickup) {
					// Multhithreading magic — who knows when the hero finishes dropping the items?
					manaAbusePickup = std::async(std::launch::async, [&, origItemStats, player, issuer]() mutable {
						Sleep(300);
					for (auto& [item, stat] : origItemStats)
						ChangeItemStatTo(item, stat, player, issuer);

					for (auto& item : ctx.physicalItems) { // wtf is with this indentation???
						if (IsWithinRadius(item->GetPos(), ctx.assignedHero->GetPos(), 50))
							oPrepareUnitOrders(player, DOTA_UNIT_ORDER_PICKUP_ITEM, item->GetIndex(), &Vector::Zero, 0, DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, issuer, true, false);
					}
					ctx.physicalItems.clear();
						});
				}
				break;
			}
			}

		if (orderType == DOTA_UNIT_ORDER_CAST_NO_TARGET ||
			orderType == DOTA_UNIT_ORDER_CAST_POSITION)
			giveOrder = !Modules::BadCastPrevention.IsBadCast(abilityIndex, position, issuer);

		if (giveOrder)
			oPrepareUnitOrders(player, orderType, targetIndex, position, abilityIndex, orderIssuer, issuer, queue, showEffects);
	}
}