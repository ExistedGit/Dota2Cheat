#pragma once
#include "../Globals/Interfaces.h"
#include "CBaseEntity.h"
#include "CDOTAItem.h"
#include "CDOTAUnitInventory.h"
#include "CDOTAModifierManager.h"
#include "../Enums.h"

class CDOTABaseNPC : public CBaseEntity {
public:
	CDOTAModifierManager* GetModifierManager() {
		// Inlined into the object instead of a pointer
		return MemberInline<CDOTAModifierManager>(Netvars::C_DOTA_BaseNPC::m_ModifierManager);
	}

	float GetMagicalArmorValue() {
		return CallVFunc<VTableIndexes::CDOTABaseNPC::GetMagicalArmorValue, float>();
	}

	// Wrapper function combining the following conditions: 
	// Is not dormant
	// Is alive
	// Is not waiting to spawn
	bool IsTargetable() {
		return !GetIdentity()->IsDormant() && GetLifeState() == 0 && !IsWaitingToSpawn();
	}

	// JS func, uses another vtable at 0xA10
	bool IsRoshan() {
		return MemberInline<VClass>(0xA10)->CallVFunc<57, bool>();
	}

	int GetAttackDamageMin() {
		return Member<int>(Netvars::C_DOTA_BaseNPC::m_iDamageMin)
			+ Member<int>(Netvars::C_DOTA_BaseNPC::m_iDamageBonus);
	}

	GETTER(bool, IsWaitingToSpawn, Netvars::C_DOTA_BaseNPC::m_bIsWaitingToSpawn);
	GETTER(bool, IsAncient, Netvars::C_DOTA_BaseNPC::m_bIsAncient);
	GETTER(float, GetArmor, Netvars::C_DOTA_BaseNPC::m_flPhysicalArmorValue);
	GETTER(float, GetBaseMagicalResistance, Netvars::C_DOTA_BaseNPC::m_flMagicalResistanceValue);
	GETTER(int, GetAttackRange, Netvars::C_DOTA_BaseNPC::m_iAttackRange);
	GETTER(const char*, GetUnitName, Netvars::C_DOTA_BaseNPC::m_iszUnitName);
	GETTER(AttackCapabilities, GetAttackCapabilities, Netvars::C_DOTA_BaseNPC::m_iAttackCapabilities);
	GETTER(float, GetHealthRegen, Netvars::C_DOTA_BaseNPC::m_flHealthThinkRegen);
	GETTER(float, GetMana, Netvars::C_DOTA_BaseNPC::m_flMana);
	GETTER(float, GetMaxMana, Netvars::C_DOTA_BaseNPC::m_flMaxMana);
	GETTER(float, GetManaRegen, Netvars::C_DOTA_BaseNPC::m_flManaThinkRegen);
	GETTER(float, GetBaseAttackTime, Netvars::C_DOTA_BaseNPC::m_flBaseAttackTime);
	GETTER(float, GetHullRadius, Netvars::C_DOTA_BaseNPC::m_flHullRadius);
	GETTER(int, GetMoveSpeed, Netvars::C_DOTA_BaseNPC::m_iMoveSpeed);
	GETTER(bool, IsMoving, Netvars::C_DOTA_BaseNPC::m_bIsMoving);

	[[nodiscard]]
	std::vector<CDOTABaseAbility*> GetAbilities();

	CDOTABaseAbility* GetAbility(int index)
	{
		if (index < 0 || index >= 35)
			return nullptr;
		auto handle = MemberInline<ENT_HANDLE>(Netvars::C_DOTA_BaseNPC::m_hAbilities)[index];
		return Interfaces::EntitySystem->GetEntity<CDOTABaseAbility>(H2IDX(handle));
	}
	CDOTABaseAbility* GetAbility(const std::string_view& name)
	{
		auto abilities = GetAbilities();
		for (auto& ability : abilities)
			if (name == ability->GetIdentity()->GetName())
				return ability;
		return nullptr;
	}


	CDOTAItem* FindItemBySubstring(const char* str);

	CDOTAUnitInventory* GetInventory();

	[[nodiscard]]
	std::vector<CDOTAItem*> GetItems();

	bool HasState(ModifierState state);
	float GetAttackSpeed();

	// This checks for modifier states under which you cannot give orders to the hero
	bool IsDisabled() {
		return HasState(MODIFIER_STATE_FEARED)
			|| HasState(MODIFIER_STATE_HEXED)
			|| HasState(MODIFIER_STATE_MUTED)
			|| HasState(MODIFIER_STATE_NIGHTMARED)
			|| HasState(MODIFIER_STATE_OUT_OF_GAME)
			|| HasState(MODIFIER_STATE_SILENCED)
			|| HasState(MODIFIER_STATE_STUNNED)
			|| HasState(MODIFIER_STATE_TAUNTED);
	}

	bool CanUseAbility(CDOTABaseAbility* ability) {
		for (auto& ab: GetAbilities()) {
			if (ab->Member<float>(Netvars::C_DOTABaseAbility::m_flChannelStartTime))
				return false;
		}
		return GetLifeState() == 0 &&
			ability->GetLevel() > 0
			&& !IsDisabled()
			&& !ability->GetCooldown()
			&& ability->GetManaCost() <= GetMana()
			&& !ability->IsInAbilityPhase();
	}

	static void BindLua(sol::state& lua);
};
