#pragma once
#include <future>
#include "../Signatures.h"
#include "../BadCastPrevention.h"

namespace Hooks {

	inline Signatures::PrepareUnitOrdersFn oPrepareUnitOrders = nullptr;
	inline std::future<void> manaAbusePickup;
	inline void hkPrepareUnitOrders(DotaPlayer* player, dotaunitorder_t orderType, UINT32 targetIndex, Vector3* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, BaseEntity* issuer, bool queue, bool showEffects) {
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
				//Redirects spell casts from illusions to the real hero
				auto npc = Interfaces::EntitySystem->GetEntity<BaseNpc>(targetIndex);

				if (strstr(npc->SchemaBinding()->binaryName, "C_DOTA_Unit_Hero") &&
					reinterpret_cast<BaseNpcHero*>(npc)->IsIllusion()) {
					auto illusionOwner = Interfaces::EntitySystem->GetEntity<DotaPlayer>(
						H2IDX(
							npc->GetOwnerEntityHandle()
						)
						)
						->GetAssignedHero();
					if (illusionOwner->IsTargetable()) {
						targetIndex =
							H2IDX(
								illusionOwner
								->GetIdentity()
								->entHandle
							);
						//showEffects = false;
					}
				}
				break;
			}
			case DOTA_UNIT_ORDER_CAST_POSITION: {
				// Blink overshoot bypass
				auto item = Interfaces::EntitySystem->GetEntity<BaseAbility>(abilityIndex);
				if (strstr(item->GetIdentity()->GetName(), "blink")) {
					auto maxDist = item->GetEffectiveCastRange();
					auto pos2D = *(Vector2*)position;
					if (!IsWithinRadius(issuer->GetPos2D(), pos2D, maxDist)) {
						auto dist = issuer->GetPos2D().DistanceTo(pos2D);
						// Relative vector from the hero to the click point
						auto vec = Vector2(pos2D.x - issuer->GetPos2D().x, pos2D.y - issuer->GetPos2D().y);
						// -1% to make it 100% be inside the radius
						vec.x *= maxDist / dist * 0.99;
						vec.x += issuer->GetPos2D().x;
						vec.y *= maxDist / dist * 0.99;
						vec.y += issuer->GetPos2D().y;

						position->x = vec.x;
						position->y = vec.y;
						showEffects = false;
					}
				}
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
					->GetEntity<BaseAbility>(abilityIndex)
					->GetIdentity()
					->GetName(),
					filters))
					break;

				BaseNpc* npc = (BaseNpc*)issuer;
				bool callPickup = false;
				uint32_t stashSlot = 6;
				for (auto& item : npc->GetItems()) {
					if (
						H2IDX(item.handle) == abilityIndex                   // must not be the item we're using
						|| npc->GetInventory()->GetItemSlot(item.handle) > 5 // must not be in the backpack
						)
						continue;

					double anyBonus = 0;
					for (auto& bonus : bonusTypes) {
						anyBonus += Signatures::Scripts::GetLevelSpecialValueFor(nullptr, H2IDX(item.handle), bonus, -1);
						if (anyBonus > 0)
							break;
					}
					auto fVec = npc->GetForwardVector(5);
					if (anyBonus > 0) {
						//std::cout << abilityIndex << bonusInt << '\n';
						queue = true;
						oPrepareUnitOrders(player, DOTA_UNIT_ORDER_DROP_ITEM, 0, &fVec, H2IDX(item.handle), DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, issuer, true, false);
						callPickup = true;
					}
				}
				if (callPickup)
					// Multhithreading magic — who knows when the hero finishes dropping the items?
					manaAbusePickup = std::async(std::launch::async, [&, player, issuer]() mutable {
					Sleep(300);
				for (auto& item : ctx.physicalItems) { // wtf is with this indentation???
					if (IsWithinRadius(item->GetPos2D(), ctx.assignedHero->GetPos2D(), 50))
						oPrepareUnitOrders(player, DOTA_UNIT_ORDER_PICKUP_ITEM, H2IDX(item->GetIdentity()->entHandle), &Vector3::Zero, 0, DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, issuer, true, false);
				}
				ctx.physicalItems.clear();
						});
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