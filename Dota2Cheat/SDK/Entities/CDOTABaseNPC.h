#pragma once
#include "CBaseEntity.h"
#include "CDOTAItem.h"
#include "CDOTAUnitInventory.h"
#include "CDOTAModifierManager.h"
#include "../Enums.h"
#include "../Base/StringUtils.h"
#include "../Interfaces/GC/CEconWearable.h"

class CDOTABaseNPC : public CBaseEntity {
public:
	inline static float(*GetAttackSpeed)(CDOTABaseNPC* npc) = nullptr;
	// Reversed via xref "CDOTA_Hud_HealthMana::Update"
	// in the beginning after the repeated section with xrefs the hero is acquired
	// trace the variable in the call with "max_physical_barrier" to the assignment of a value where an offset to hero is used
	// that offset ultimately leads to such a structure
	// sig to opcode: "F3 45 0F 10 AE"
	struct BarrierData {
		float
			maxPhys,
			maxMagic,
			maxAll, 
			phys,  // blocks physical damage
			magic, // blocks magical damage
			all; // blocks all forms of damage
	};

	GETTER(BarrierData, GetBarriers, 0x1734);

	// GETTER(CAssetModifierContainer*, GetAssetModifierContainer, 0xa20);

	IGETTER(CDOTAModifierManager, GetModifierManager, Netvars::C_DOTA_BaseNPC::m_ModifierManager);

	CDOTAModifier* GetModifier(std::string_view name) {
		for (auto& modifier : GetModifierManager()->GetModifierList())
			if (modifier->GetName() == name)
				return modifier;

		return nullptr;
	}
	bool HasModifier(std::string_view name) {
		return GetModifier(name) != nullptr;
	}

	float GetPhysicalArmorValue() {
		return CallVFunc<VTableIndexes::CDOTABaseNPC::GetPhysicalArmorValue, float>(0ull);
	}

	float GetMagicalArmorValue() {
		return CallVFunc<VTableIndexes::CDOTABaseNPC::GetMagicalArmorValue, float>(0ull, 0ull);
	}

	VGETTER(float, GetIdealSpeed, VTableIndexes::CDOTABaseNPC::GetIdealSpeed);
	VGETTER(void, OnWearablesChanged, VTableIndexes::CDOTABaseNPC::OnWearablesChanged);

	// Wrapper function combining the following conditions: 
	// Is not dormant
	// Is alive
	// Is not waiting to spawn
	bool IsTargetable() {
		return !GetIdentity()->IsDormant() && GetLifeState() == 0 && !IsWaitingToSpawn();
	}

	// JS func, uses another vtable at offset
	bool IsRoshan() {
		return MemberInline<VClass>(0xA20)->CallVFunc<VTableIndexes::CDOTABaseNPC::IsRoshan, bool>();
	}

	int GetAttackDamageMin() {
		return Member<int>(Netvars::C_DOTA_BaseNPC::m_iDamageMin)
			+ Member<int>(Netvars::C_DOTA_BaseNPC::m_iDamageBonus);
	}

	float GetAttackRange() {
		return CallVFunc<VTableIndexes::CDOTABaseNPC::GetAttackRange, float>(0, 1);
	};

	GETTER(bool, IsWaitingToSpawn, Netvars::C_DOTA_BaseNPC::m_bIsWaitingToSpawn);
	GETTER(bool, IsAncient, Netvars::C_DOTA_BaseNPC::m_bIsAncient);
	GETTER(float, GetArmor, Netvars::C_DOTA_BaseNPC::m_flPhysicalArmorValue);
	GETTER(float, GetBaseMagicalResistance, Netvars::C_DOTA_BaseNPC::m_flMagicalResistanceValue);
	GETTER(const char*, GetUnitName, Netvars::C_DOTA_BaseNPC::m_iszUnitName);
	GETTER(DOTAUnitAttackCapability_t, GetAttackCapabilities, Netvars::C_DOTA_BaseNPC::m_iAttackCapabilities);
	GETTER(float, GetHealthRegen, Netvars::C_DOTA_BaseNPC::m_flHealthThinkRegen);
	GETTER(float, GetMana, Netvars::C_DOTA_BaseNPC::m_flMana);
	GETTER(float, GetMaxMana, Netvars::C_DOTA_BaseNPC::m_flMaxMana);
	GETTER(float, GetManaRegen, Netvars::C_DOTA_BaseNPC::m_flManaThinkRegen);
	GETTER(float, GetBaseAttackTime, Netvars::C_DOTA_BaseNPC::m_flBaseAttackTime);
	GETTER(float, GetHullRadius, Netvars::C_DOTA_BaseNPC::m_flHullRadius);
	GETTER(int, GetMoveSpeed, Netvars::C_DOTA_BaseNPC::m_iMoveSpeed);
	GETTER(bool, IsMoving, Netvars::C_DOTA_BaseNPC::m_bIsMoving);

	FIELD(CUtlVector<CHandle<CEconWearable>>, Wearables, Netvars::C_BaseCombatCharacter::m_hMyWearables);
	FIELD(CUtlVector<CHandle<CEconWearable>>, OldWearables, Netvars::C_DOTA_BaseNPC::m_hOldWearables);

	[[nodiscard]]
	std::vector<CDOTABaseAbility*> GetAbilities();

	CDOTABaseAbility* GetAbility(int index)
	{
		if (index < 0 || index >= 35)
			return nullptr;

		return MemberInline<CHandle<CDOTABaseAbility>>(Netvars::C_DOTA_BaseNPC::m_hAbilities)[index];
	}
	CDOTABaseAbility* GetAbility(const std::string_view& name)
	{
		auto abilities = GetAbilities();
		for (auto& ability : abilities)
			if (name == ability->GetIdentity()->GetName())
				return ability;
		return nullptr;
	}

	Vector GetHealthBarPos() {
		auto pos = GetPos();
		pos.z += Member<int>(Netvars::C_DOTA_BaseNPC::m_iHealthBarOffset);
		return pos;
	};

	CDOTAItem* FindItemBySubstring(const char* str) {
		for (const auto& item : GetItems())
			if (
				item
				&& item->GetIdentity()->GetName()
				&& strstr(item->GetIdentity()->GetName(), str)
				)
				return item;

		return nullptr;
	}
	CDOTAItem* FindItem(const char* str) {
		for (const auto& item : GetItems())
			if (
				item
				&& item->GetIdentity()->GetName()
				&& !strcmp(item->GetIdentity()->GetName(), str)
				)
				return item;

		return nullptr;
	}

	IGETTER(CDOTAUnitInventory, GetInventory, Netvars::C_DOTA_BaseNPC::m_Inventory);

	[[nodiscard]]
	std::vector<CDOTAItem*> GetItems();

	bool HasState(ModifierState state) {
		auto unitState = Member<int64>(Netvars::C_DOTA_BaseNPC::m_nUnitState64);
		return (unitState & (1Ui64 << (int)state));
	}

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
		for (auto& ab : GetAbilities()) {
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

};
