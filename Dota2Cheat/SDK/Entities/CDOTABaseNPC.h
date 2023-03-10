#pragma once
#include "../Globals/Interfaces.h"
#include "CBaseEntity.h"
#include "CDOTAItem.h"
#include "CDOTAUnitInventory.h"
#include "CDOTAModifierManager.h"

class CDOTABaseNPC : public CBaseEntity {
public:
	CDOTAModifierManager* GetModifierManager() {
		// Inlined into the object instead of a pointer
		return MemberInline<CDOTAModifierManager>(Netvars::C_DOTA_BaseNPC::m_ModifierManager);
	}

	// Wrapper function combining the following conditions: 
	// Is not dormant
	// Is alive
	// Is not waiting to spawn
	bool IsTargetable() {
		return !GetIdentity()->IsDormant() && GetLifeState() == 0 && !IsWaitingToSpawn();
	}
	GETTER(bool, IsWaitingToSpawn, Netvars::C_DOTA_BaseNPC::m_bIsWaitingToSpawn);
	GETTER(bool, IsAncient, Netvars::C_DOTA_BaseNPC::m_bIsAncient);

	//Implemented as a method returning a bool rather than a field
	//Is inside some kind of structure on offset BE8
	bool IsRoshan() {
		return MemberInline<VClass>(0xbe8)->CallVFunc<57, bool>();
	}

	int GetAttackDamageMin() {
		return Member<int>(Netvars::C_DOTA_BaseNPC::m_iDamageMin)
			+ Member<int>(Netvars::C_DOTA_BaseNPC::m_iDamageBonus);
	}

	GETTER(float, GetArmor, Netvars::C_DOTA_BaseNPC::m_flPhysicalArmorValue); 
	GETTER(int, GetAttackRange, Netvars::C_DOTA_BaseNPC::m_iAttackRange); 
	GETTER(ENT_HANDLE, GoalEntity, Netvars::C_DOTA_BaseNPC::m_hGoalEntity); 
	GETTER(const char*, GetUnitName, Netvars::C_DOTA_BaseNPC::m_iszUnitName);

	GETTER(float, GetMana, Netvars::C_DOTA_BaseNPC::m_flMana);
	GETTER(float, GetMaxMana, Netvars::C_DOTA_BaseNPC::m_flMaxMana);
	GETTER(float, GetManaRegen, Netvars::C_DOTA_BaseNPC::m_flManaThinkRegen);
	GETTER(float, GetBaseAttackTime, Netvars::C_DOTA_BaseNPC::m_flBaseAttackTime);
	GETTER(float, GetHullRadius, Netvars::C_DOTA_BaseNPC::m_flHullRadius);

	[[nodiscard]]
	std::vector<CDOTABaseAbility*> GetAbilities() {
		std::vector<CDOTABaseAbility*> result{};
		ENT_HANDLE* hAbilities = MemberInline<ENT_HANDLE>(Netvars::C_DOTA_BaseNPC::m_hAbilities);
		for (int j = 0; j < 35; j++) {
			ENT_HANDLE handle = hAbilities[j];
			if (!HVALID(handle))
				continue;

			result.push_back(Interfaces::EntitySystem->GetEntity<CDOTABaseAbility>(
				H2IDX(handle)
				));
		}
		return result;
	}

	CDOTAItem* FindItemBySubstring(const char* str) {
		if (!str)
			return nullptr;

		for (const auto& item : GetItems())
			if (
				item
				&& item->GetIdentity()->GetName()
				&& strstr(item->GetIdentity()->GetName(), str)
				)
				return item;

		return nullptr;
	}

	CDOTAUnitInventory* GetInventory() {
		return MemberInline<CDOTAUnitInventory>(Netvars::C_DOTA_BaseNPC::m_Inventory);
	}

	[[nodiscard]]
	std::vector<CDOTAItem*> GetItems() {
		std::vector<CDOTAItem*> result{};
		CDOTAUnitInventory* inv = GetInventory();
		if (!inv)
			return result;

		auto itemsHandle = inv->GetItems();
		for (auto& handle : itemsHandle) {
			if (!HVALID(handle))
				continue;

			result.push_back(Interfaces::EntitySystem->GetEntity<CDOTAItem>(H2IDX(handle)));
		}

		return result;
	}



	float GetAttackSpeed() {
		return 1 + CallVFunc<295, float>();
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CDOTABaseNPC>("CDOTABaseNPC", sol::base_classes, sol::bases<CBaseEntity>());

		type["GetModifierManager"] = &CDOTABaseNPC::GetModifierManager;
		type["IsTargetable"] = &CDOTABaseNPC::IsTargetable;
		type["IsWaitingToSpawn"] = &CDOTABaseNPC::IsWaitingToSpawn;
		type["IsAncient"] = &CDOTABaseNPC::IsAncient;
		type["IsRoshan"] = &CDOTABaseNPC::IsRoshan;
		type["GetAttackRange"] = &CDOTABaseNPC::GetAttackRange;
		type["GoalEntity"] = &CDOTABaseNPC::GoalEntity;
		type["GetUnitName"] = &CDOTABaseNPC::GetUnitName;
		type["GetAbilities"] = &CDOTABaseNPC::GetAbilities;
		type["FindItemBySubstring"] = &CDOTABaseNPC::FindItemBySubstring;
		type["GetInventory"] = &CDOTABaseNPC::GetInventory;
		type["GetItems"] = &CDOTABaseNPC::GetItems;;
		type["GetMana"] = &CDOTABaseNPC::GetMana;
		type["GetMaxMana"] = &CDOTABaseNPC::GetMaxMana;
		type["GetBaseAttackTime"] = &CDOTABaseNPC::GetBaseAttackTime;
		type["GetHullRadius"] = &CDOTABaseNPC::GetHullRadius;
		type["GetAttackSpeed"] = &CDOTABaseNPC::GetAttackSpeed;
		lua["CDOTABaseNPC"] = [](CBaseEntity* ent) { return (CDOTABaseNPC*)ent; };
	}
};
