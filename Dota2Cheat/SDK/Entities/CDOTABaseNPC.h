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
	GETTER(AttackCapabilities, GetAttackCapabilities, Netvars::C_DOTA_BaseNPC::m_iAttackCapabilities);
	GETTER(float, GetMana, Netvars::C_DOTA_BaseNPC::m_flMana);
	GETTER(float, GetMaxMana, Netvars::C_DOTA_BaseNPC::m_flMaxMana);
	GETTER(float, GetManaRegen, Netvars::C_DOTA_BaseNPC::m_flManaThinkRegen);
	GETTER(float, GetBaseAttackTime, Netvars::C_DOTA_BaseNPC::m_flBaseAttackTime);
	GETTER(float, GetHullRadius, Netvars::C_DOTA_BaseNPC::m_flHullRadius);

	[[nodiscard]]
	std::vector<CDOTABaseAbility*> GetAbilities();

	CDOTAItem* FindItemBySubstring(const char* str);

	CDOTAUnitInventory* GetInventory();

	[[nodiscard]]
	std::vector<CDOTAItem*> GetItems();

	bool HasState(ModifierState state);

	float GetAttackSpeed();

	static void BindLua(sol::state& lua);
};
