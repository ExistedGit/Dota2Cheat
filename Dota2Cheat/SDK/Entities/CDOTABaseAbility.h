#pragma once
#include "CBaseEntity.h"
#include "../Base/CUtlVector.h"

// structs and enums from source2sdk aka NL dumper

enum class EDOTASpecialBonusOperation : uint8_t
{
	SPECIAL_BONUS_ADD = 0x0,
	SPECIAL_BONUS_MULTIPLY = 0x1,
	SPECIAL_BONUS_SUBTRACT = 0x2,
	SPECIAL_BONUS_PERCENTAGE_ADD = 0x3,
	SPECIAL_BONUS_PERCENTAGE_SUBTRACT = 0x4,
	SPECIAL_BONUS_SET = 0x5,
};

struct DOTASpecialAbility_t
{
public:
	struct DOTALevelingAbilityBonus_t
	{
		char* m_pszName; // 0x0	
		CUtlVector<float> m_vecValues; // 0x8	
		EDOTASpecialBonusOperation m_eOperation; // 0x20	
	};
	struct DOTASpecialAbilityBonus_t
	{
		char* m_pszName; // 0x0	
		float m_fValue; // 0x8	
		EDOTASpecialBonusOperation m_eOperation; // 0xc	
	};

	char* m_pszName; // 0x0	
	char* m_pszValue; // 0x8	
	char* m_pszLevelKey; // 0x10	
	char* m_pszSpecialBonusAbility; // 0x18	
	char* m_pszSpecialBonusField; // 0x20	
	char* m_pszSpecialBonusADLinkedAbilities; // 0x28	
	int32_t m_nCount; // 0x30	
	float m_Values[20]; // 0x34	
	int32_t m_nBonusesCount; // 0x84	
	DOTASpecialAbilityBonus_t m_Bonuses[4]; // 0x88	
	DOTALevelingAbilityBonus_t m_ScepterBonus; // 0xc8	
	DOTALevelingAbilityBonus_t m_ShardBonus; // 0xf0	
	int32_t m_nDamageTypeField; // 0x118	
	bool m_bSpellDamageField; // 0x11c	
	bool m_bScepterField; // 0x11d	
	bool m_bShardField; // 0x11e	
	EDOTASpecialBonusOperation m_eSpecialBonusOperation; // 0x11f	
};

// Alignment: 86
// Size: 0x248
struct DOTAAbilityDefinition_t
{
public:
	typedef uint32_t DotaGestureSlot_t;
	typedef uint32_t AbilityID_t;
	typedef uint32_t ItemQuality_t;



	char* m_pszAbilityName; // 0x0	
	char* m_pszTextureName; // 0x8	
	char* m_pszSharedCooldownName; // 0x10	
	char* m_pszKeyOverride; // 0x18	
	char* m_pszItemRecipeName; // 0x20	
	char* m_pszLinkedAbility; // 0x28	
	int32_t m_castActivity; // 0x30	
	DotaGestureSlot_t m_castActivityGestureSlot; // 0x34	
	void* m_pKVData; // 0x38	
	AbilityID_t m_iAbilityID; // 0x40	
	int32_t m_iAbilityType; // 0x44	
	uint64_t m_iAbilityBehavior; // 0x48	
	int32_t m_iAbilityTargetTeam; // 0x50	
	int32_t m_iAbilityTargetType; // 0x54	
	int32_t m_iAbilityTargetFlags; // 0x58	
	int32_t m_iAbilityDamageType; // 0x5c	
	int32_t m_iAbilityImmunityType; // 0x60	
	int32_t m_iAbilityDispellableType; // 0x64	
	int32_t m_iFightRecapLevel; // 0x68	
	int32_t m_iTokenTier; // 0x6c	
	PAD(4);// item_definition_index_t m_iAssociatedConsumableItemDef; // 0x70	
	uint32_t m_nRequiredEffectsMask; // 0x74	
	int32_t m_iAssociatedEventID; // 0x78	
	int32_t m_iMaxLevel; // 0x7c	
	int32_t m_iItemBaseLevel; // 0x80	
	float m_flItemLevelByGameTime; // 0x84	
	int32_t m_iItemCost; // 0x88	
	int32_t m_iItemInitialCharges; // 0x8c	
	int32_t m_iItemNeutralTierIndex; // 0x90	
	int32_t m_iItemStockMax; // 0x94	
	float m_fItemStockTime; // 0x98	
private:
	[[maybe_unused]] uint8_t __pad009c[0x4]; // 0x9c
public:
	int32* m_pItemShopTagKeys; // 0xa0	
	AbilityID_t m_nRecipeResultAbilityID; // 0xa8	
private:
	[[maybe_unused]] uint8_t __pad00ac[0x4]; // 0xac
public:
	CUtlVector< AbilityID_t > m_vecItemCombinesInto; // 0xb0	
	PAD(24); // CUtlVector< ItemRecipe_t > m_vecItemRecipes; // 0xc8	
	int32_t m_nUpgradeGoal; // 0xe0	
	ItemQuality_t m_ItemQuality; // 0xe4	
	float m_flModifierValue; // 0xe8	
	float m_flModifierValueBonus; // 0xec	
	PAD(24); // CUtlVector< CUtlString > m_InvalidHeroes; // 0xf0	
private:
	[[maybe_unused]] uint8_t __pad0108[0xcc]; // 0x108
public:
	bool m_bHasScepterUpgrade; // 0x1d4	
	bool m_bHasShardUpgrade; // 0x1d5	
	bool m_bSharedWithTeammates; // 0x1d6	
private:
	[[maybe_unused]] uint8_t __pad01d7[0x1]; // 0x1d7
public:
	int32_t m_nCastRangeBuffer; // 0x1d8	
	int32_t m_nSpecialAbilities; // 0x1dc	
	DOTASpecialAbility_t* m_pSpecialAbilities; // 0x1e0	
	PAD(24); // CUtlVector< DOTAOutgoingBonus_t > m_OutgoingBonuses; // 0x1e8	
	char* m_pModelName; // 0x200	
	char* m_pModelAlternateName; // 0x208	
	char* m_pEffectName; // 0x210	
	char* m_pPingOverrideText; // 0x218	
	char* m_pszRequiredCustomShopName; // 0x220	
	char* m_pszLinkedScepterAbility; // 0x228	
	char* m_pszLinkedShardAbility; // 0x230	
	int32_t m_nKillToastOverride; // 0x238	
	int32_t m_iLevelsBetweenUpgrades; // 0x23c	
	int32_t m_iRequiredLevel; // 0x240	
	struct
	{
		uint8_t m_bIsItem : 1;
		uint8_t m_bItemIsRecipe : 1;
		uint8_t m_bItemIsRecipeGenerated : 1;
		uint8_t m_bItemRecipeConsumesCharges : 1;
		uint8_t m_bItemAvailableAtSecretShop : 1;
		uint8_t m_bItemAvailableAtGlobalShop : 1;
		uint8_t m_bItemAvailableAtSideShop : 1;
		uint8_t m_bItemAvailableAtCustomShop : 1;
		uint8_t m_bItemIsPureSupport : 1;
		uint8_t m_bItemIsPurchasable : 1;
		uint8_t m_bItemIsUpgradeable : 1;
		uint8_t m_bItemStackable : 1;
		uint8_t m_bDisplayAdditionalHeroes : 1;
		uint8_t m_bItemContributesToNetWorthWhenDropped : 1;
		uint8_t m_bOnCastbar : 1;
		uint8_t m_bOnLearnbar : 1;
		uint8_t m_bIsGrantedByScepter : 1;
		uint8_t m_bIsGrantedByShard : 1;
		uint8_t m_bIsCastableWhileHidden : 1;
		uint8_t m_bAnimationIgnoresModelScale : 1;
		uint8_t m_bIsPlayerSpecificCooldown : 1;
		uint8_t m_bIsAllowedInBackpack : 1;
		uint8_t m_bIsObsolete : 1;
		uint8_t m_bItemRequiresCustomShop : 1;
		uint8_t m_bShouldBeSuggested : 1;
		uint8_t m_bShouldBeInitiallySuggested : 1;
		uint8_t m_bHasCastAnimation : 1;
		uint8_t m_bItemHasPassive : 1;
		uint8_t m_bDisplayOverheadAlertOnReceived : 1;
		uint8_t __pad2 : 5;
	}; // 34 bits

	DOTASpecialAbility_t* FindSpecialValue(const char* name) {
		for (int i = 0; i < m_nSpecialAbilities; i++)
			if (!strcmp(m_pSpecialAbilities[i].m_pszName, name))
				return m_pSpecialAbilities + i;
		return nullptr;
	}
};

class CDOTABaseAbility : public CBaseEntity {
public:
	using GetLevelSpecialValueForFn = float(__fastcall*)(void* thisptr, const char* value, int level, void* unk1, bool noOverride, bool* result);
	static inline GetLevelSpecialValueForFn GetLevelSpecialValueForFunc{};

	template<typename T = float>
	T GetLevelSpecialValueFor(const char* value, int level = -1) {
		return (T)GetLevelSpecialValueForFunc(this, value, level, nullptr, 0, nullptr);
	}

	GETTER(bool, IsToggled, Netvars::C_DOTABaseAbility::m_bToggleState); // For things like Pudge's Rot or Armlet
	GETTER(bool, IsHidden, Netvars::C_DOTABaseAbility::m_bHidden);
	GETTER(float, GetCooldown, Netvars::C_DOTABaseAbility::m_fCooldown);
	GETTER(float, GetCooldownLength, Netvars::C_DOTABaseAbility::m_flCooldownLength);
	GETTER(int, GetLevel, Netvars::C_DOTABaseAbility::m_iLevel);
	GETTER(int, GetCharges, Netvars::C_DOTABaseAbility::m_nAbilityCurrentCharges);
	GETTER(float, GetChargeRestoreCooldown, Netvars::C_DOTABaseAbility::m_fAbilityChargeRestoreTimeRemaining);
	GETTER(int, GetManaCost, Netvars::C_DOTABaseAbility::m_iManaCost);
	GETTER(bool, IsInAbilityPhase, Netvars::C_DOTABaseAbility::m_bInAbilityPhase);
	GETTER(DOTAAbilityDefinition_t*, GetDefinition, 0x538);

	int GetMaxLevel() {
		return GetDefinition()->m_iMaxLevel;
	};

#define SPECVAL_GETTER(type, name, val) type name() { return GetLevelSpecialValueFor<type>(val); }

	SPECVAL_GETTER(int, GetCastRange, "AbilityCastRange");
	SPECVAL_GETTER(int, GetAOERadius, "radius");

#undef SPECVAL_GETTER

	// JS Func
	int GetEffectiveCastRange() {
		return CallVFunc<VTableIndexes::CDOTABaseAbility::GetEffectiveCastRange, int>(nullptr, nullptr, nullptr);
	}

};