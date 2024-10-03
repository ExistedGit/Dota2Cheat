#pragma once
#include "CBaseEntity.h"
#include "../Base/CUtlVector.h"

// structs and enums below from source2sdk aka NL dumper
// I think I should get around to generating them myself

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

// from moddota lua api docs
enum ABILITY_TYPES {
	ABILITY_TYPE_BASIC = 0,
	ABILITY_TYPE_ULTIMATE = 1,
	ABILITY_TYPE_ATTRIBUTES = 2,
	ABILITY_TYPE_HIDDEN = 3
};
enum AbilityBarType_t : std::uint32_t
{
	ABILITY_BAR_TYPE_MAIN = 0x0,
	ABILITY_BAR_TYPE_SECONDARY = 0x1,
	ABILITY_BAR_TYPE_TERTIARY = 0x2,
};
enum DOTA_ABILITY_BEHAVIOR : std::uint64_t
{
	DOTA_ABILITY_BEHAVIOR_NONE = 0x0,
	DOTA_ABILITY_BEHAVIOR_HIDDEN = 0x1,
	DOTA_ABILITY_BEHAVIOR_PASSIVE = 0x2,
	DOTA_ABILITY_BEHAVIOR_NO_TARGET = 0x4,
	DOTA_ABILITY_BEHAVIOR_UNIT_TARGET = 0x8,
	DOTA_ABILITY_BEHAVIOR_POINT = 0x10,
	DOTA_ABILITY_BEHAVIOR_AOE = 0x20,
	DOTA_ABILITY_BEHAVIOR_NOT_LEARNABLE = 0x40,
	DOTA_ABILITY_BEHAVIOR_CHANNELLED = 0x80,
	DOTA_ABILITY_BEHAVIOR_ITEM = 0x100,
	DOTA_ABILITY_BEHAVIOR_TOGGLE = 0x200,
	DOTA_ABILITY_BEHAVIOR_DIRECTIONAL = 0x400,
	DOTA_ABILITY_BEHAVIOR_IMMEDIATE = 0x800,
	DOTA_ABILITY_BEHAVIOR_AUTOCAST = 0x1000,
	DOTA_ABILITY_BEHAVIOR_OPTIONAL_UNIT_TARGET = 0x2000,
	DOTA_ABILITY_BEHAVIOR_OPTIONAL_POINT = 0x4000,
	DOTA_ABILITY_BEHAVIOR_OPTIONAL_NO_TARGET = 0x8000,
	DOTA_ABILITY_BEHAVIOR_AURA = 0x10000,
	DOTA_ABILITY_BEHAVIOR_ATTACK = 0x20000,
	DOTA_ABILITY_BEHAVIOR_DONT_RESUME_MOVEMENT = 0x40000,
	DOTA_ABILITY_BEHAVIOR_ROOT_DISABLES = 0x80000,
	DOTA_ABILITY_BEHAVIOR_UNRESTRICTED = 0x100000,
	DOTA_ABILITY_BEHAVIOR_IGNORE_PSEUDO_QUEUE = 0x200000,
	DOTA_ABILITY_BEHAVIOR_IGNORE_CHANNEL = 0x400000,
	DOTA_ABILITY_BEHAVIOR_DONT_CANCEL_MOVEMENT = 0x800000,
	DOTA_ABILITY_BEHAVIOR_DONT_ALERT_TARGET = 0x1000000,
	DOTA_ABILITY_BEHAVIOR_DONT_RESUME_ATTACK = 0x2000000,
	DOTA_ABILITY_BEHAVIOR_NORMAL_WHEN_STOLEN = 0x4000000,
	DOTA_ABILITY_BEHAVIOR_IGNORE_BACKSWING = 0x8000000,
	DOTA_ABILITY_BEHAVIOR_RUNE_TARGET = 0x10000000,
	DOTA_ABILITY_BEHAVIOR_DONT_CANCEL_CHANNEL = 0x20000000,
	DOTA_ABILITY_BEHAVIOR_VECTOR_TARGETING = 0x40000000,
	DOTA_ABILITY_BEHAVIOR_LAST_RESORT_POINT = 0x80000000,
	DOTA_ABILITY_BEHAVIOR_CAN_SELF_CAST = 0x100000000,
	DOTA_ABILITY_BEHAVIOR_SHOW_IN_GUIDES = 0x200000000,
	DOTA_ABILITY_BEHAVIOR_UNLOCKED_BY_EFFECT_INDEX = 0x400000000,
	DOTA_ABILITY_BEHAVIOR_SUPPRESS_ASSOCIATED_CONSUMABLE = 0x800000000,
	DOTA_ABILITY_BEHAVIOR_FREE_DRAW_TARGETING = 0x1000000000,
	DOTA_ABILITY_BEHAVIOR_IGNORE_SILENCE = 0x2000000000,
	DOTA_ABILITY_BEHAVIOR_OVERSHOOT = 0x4000000000,
	DOTA_ABILITY_BEHAVIOR_IGNORE_MUTED = 0x8000000000,
	DOTA_ABILITY_BEHAVIOR_ALT_CASTABLE = 0x10000000000,
	DOTA_ABILITY_BEHAVIOR_SKIP_FOR_KEYBINDS = 0x40000000000,
	DOTA_ABILITY_BEHAVIOR_INNATE_UI = 0x80000000000,
	DOTA_ABILITY_BEHAVIOR_UNSWAPPABLE = 0x100000000000,
	DOTA_ABILITY_BEHAVIOR_DONT_PROC_OTHER_ABILITIES = 0x200000000000,
	DOTA_ABILITY_BEHAVIOR_IGNORE_INVISIBLE = 0x400000000000,
	DOTA_ABILITY_BEHAVIOR_AFFECTED_BY_MUTE = 0x800000000000,
	DOTA_ABILITY_BEHAVIOR_IS_FAKE_ITEM = 0x1000000000000,
};

#pragma pack(push, 1)
struct DOTAAbilityDefinition_t
{
public:
	char* m_pszAbilityName; // 0x0        
	char* m_pszTextureName; // 0x8        
	char* m_pszSharedCooldownName; // 0x10        
	char* m_pszKeyOverride; // 0x18        
	char* m_pszItemRecipeName; // 0x20        
	char* m_pszLinkedAbility; // 0x28        
	int32_t m_castActivity; // 0x30        
	/* DotaGestureSlot_t */ int m_castActivityGestureSlot; // 0x34        
	KeyValues* m_pKVData; // 0x38        
	/* AbilityID_t */ int m_iAbilityID; // 0x40        
	ABILITY_TYPES m_iAbilityType; // 0x44        
	DOTA_ABILITY_BEHAVIOR m_iAbilityBehavior; // 0x48        
	int32_t m_iAbilityTargetTeam; // 0x50        
	int32_t m_iAbilityTargetType; // 0x54        
	int32_t m_iAbilityTargetFlags; // 0x58        
	int32_t m_iAbilityDamageType; // 0x5c        
	int32_t m_iAbilityImmunityType; // 0x60        
	int32_t m_iAbilityDispellableType; // 0x64        
	int32_t m_iFightRecapLevel; // 0x68        
	int32_t m_iTokenTier; // 0x6c        
	/* item_definition_index_t*/ int m_iAssociatedConsumableItemDef; // 0x70        
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
	[[maybe_unused]] std::uint8_t pad_0x9c[0x4]; // 0x9c
	int32_t* m_pItemShopTagKeys; // 0xa0        
	/*AbilityID_t */ int m_nRecipeResultAbilityID; // 0xa8        
	[[maybe_unused]] std::uint8_t pad_0xac[0x4]; // 0xac
	// m_vecItemCombinesInto has a template type with potentially unknown template parameters. You can try uncommenting the field below.
	// CUtlVector<client::AbilityID_t> m_vecItemCombinesInto;
	char m_vecItemCombinesInto[0x18]; // 0xb0        
	// m_vecItemRecipes has a template type with potentially unknown template parameters. You can try uncommenting the field below.
	// CUtlVector<client::ItemRecipe_t> m_vecItemRecipes;
	char m_vecItemRecipes[0x18]; // 0xc8        
	int32_t m_nUpgradeGoal; // 0xe0        
	/* ItemQuality_t */ int m_ItemQuality; // 0xe4        
	float m_flModifierValue; // 0xe8        
	float m_flModifierValueBonus; // 0xec        
	// m_InvalidHeroes has a template type with potentially unknown template parameters. You can try uncommenting the field below.
	// CUtlVector<CUtlString> m_InvalidHeroes;
	char m_InvalidHeroes[0x18]; // 0xf0        
	[[maybe_unused]] std::uint8_t pad_0x108[0xcc]; // 0x108
	bool m_bHasScepterUpgrade; // 0x1d4        
	bool m_bHasShardUpgrade; // 0x1d5        
	bool m_bSharedWithTeammates; // 0x1d6        
	[[maybe_unused]] std::uint8_t pad_0x1d7[0x1]; // 0x1d7
	int32_t m_nCastRangeBuffer; // 0x1d8        
	int32_t m_nSpecialAbilities; // 0x1dc        
	DOTASpecialAbility_t* m_pSpecialAbilities; // 0x1e0        
	// m_OutgoingBonuses has a template type with potentially unknown template parameters. You can try uncommenting the field below.
	// CUtlVector<client::DOTAOutgoingBonus_t> m_OutgoingBonuses;
	char m_OutgoingBonuses[0x18]; // 0x1e8        
	char* m_pModelName; // 0x200        
	char* m_pModelAlternateName; // 0x208        
	char* m_pEffectName; // 0x210        
	char* m_pPingOverrideText; // 0x218        
	char* m_pszRequiredCustomShopName; // 0x220        
	char* m_pszLinkedScepterAbility; // 0x228        
	char* m_pszLinkedShardAbility; // 0x230        
	char* m_pszDependentOnAbility; // 0x238        
	int32_t m_nKillToastOverride; // 0x240        
	int32_t m_iLevelsBetweenUpgrades; // 0x244        
	int32_t m_iRequiredLevel; // 0x248        
	// start of bitfield block at 0x24c
	uint64_t m_bIsItem : 1;
	uint64_t m_bItemIsRecipe : 1;
	uint64_t m_bItemIsRecipeGenerated : 1;
	uint64_t m_bItemRecipeConsumesCharges : 1;
	uint64_t m_bItemAvailableAtSecretShop : 1;
	uint64_t m_bItemAvailableAtGlobalShop : 1;
	uint64_t m_bItemAvailableAtSideShop : 1;
	uint64_t m_bItemAvailableAtCustomShop : 1;

	uint64_t m_bItemIsPureSupport : 1;
	uint64_t m_bItemIsPurchasable : 1;
	uint64_t m_bItemIsUpgradeable : 1;
	uint64_t m_bItemStackable : 1;
	uint64_t m_bDisplayAdditionalHeroes : 1;
	uint64_t m_bItemContributesToNetWorthWhenDropped : 1;
	uint64_t m_bOnCastbar : 1;
	uint64_t m_bOnLearnbar : 1;

	uint64_t m_bIsGrantedByScepter : 1;
	uint64_t m_bIsGrantedByShard : 1;
	uint64_t m_bIsCastableWhileHidden : 1;
	uint64_t m_bAnimationIgnoresModelScale : 1;
	uint64_t m_bIsPlayerSpecificCooldown : 1;
	uint64_t m_bIsAllowedInBackpack : 1;
	uint64_t m_bIsObsolete : 1;
	uint64_t m_bItemRequiresCustomShop : 1;

	uint64_t m_bSuggestPregame : 1;
	uint64_t m_bSuggestEarlygame : 1;
	uint64_t m_bSuggestLategame : 1;
	uint64_t m_bIsAffectedByAoEIncrease : 1;
	uint64_t m_bHasCastAnimation : 1;
	uint64_t m_bItemHasPassive : 1;
	uint64_t m_bDisplayOverheadAlertOnReceived : 1;
	uint64_t m_bInnate : 1;

	uint64_t m_bIsSpeciallyAllowedInNeutralSlot : 1;
	uint64_t m_bIsSpeciallyBannedFromNeutralSlot : 1;
	uint64_t m_bIsCooldownPausedOutOfInventory : 1;
	uint64_t m_bAllowCombineFromGround : 1;
	uint64_t m_bRestrictToMaxLevel : 1;
	uint64_t m_bHasDynamicValue : 1;
	uint64_t m_bBreakable : 1;
	// end of bitfield block // 39 bits
	[[maybe_unused]] std::uint8_t pad_0x254[0x4];
};
#pragma pack(pop)

static_assert(sizeof(DOTAAbilityDefinition_t) == 0x258);

class CDOTABaseAbility : public CBaseEntity {
public:
	using GetLevelSpecialValueForFn = float(__fastcall*)(const void* thisptr, const char* value, int level, void* unk1, bool noOverride, bool* result);
	static inline GetLevelSpecialValueForFn GetLevelSpecialValueForFunc{};

	template<typename T = float>
	T GetLevelSpecialValueFor(std::string_view value, int level = -1) const {
		return (T)GetLevelSpecialValueForFunc(this, value.data(), level, nullptr, 0, nullptr);
	}

	GETTER(bool, IsToggled, Netvars::C_DOTABaseAbility::m_bToggleState); // For things like Pudge's Rot or Armlet
	GETTER(bool, IsHidden, Netvars::C_DOTABaseAbility::m_bHidden);
	GETTER(float, GetCooldown, Netvars::C_DOTABaseAbility::m_fCooldown);
	GETTER(float, GetCooldownLength, Netvars::C_DOTABaseAbility::m_flCooldownLength);
	GETTER(int, GetLevel, Netvars::C_DOTABaseAbility::m_iLevel);
	GETTER(int, GetCharges, Netvars::C_DOTABaseAbility::m_nAbilityCurrentCharges);
	GETTER(float, GetChargeRestoreCooldown, Netvars::C_DOTABaseAbility::m_fAbilityChargeRestoreTimeRemaining);
	GETTER(bool, IsInAbilityPhase, Netvars::C_DOTABaseAbility::m_bInAbilityPhase);
	GETTER(DOTAAbilityDefinition_t*, GetDefinition, 0x538);

	int GetMaxLevel() const {
		return GetDefinition()->m_iMaxLevel;
	};

	// Returns ability manacost if present, otherwise returns -1
	int GetManaCost(int defaultVal = -1) const {
		return GetVFunc(VMI::CDOTABaseAbility::GetManaCost).Call<int>(defaultVal);
	};

#define SPECVAL_GETTER(type, name, val) type name() const { return GetLevelSpecialValueFor<type>(val); }

	SPECVAL_GETTER(int, GetCastRange, "AbilityCastRange");
	SPECVAL_GETTER(int, GetAOERadius, "radius");

#undef SPECVAL_GETTER

	const char* GetAbilityTextureName(bool applyAssetModifiers = true) const {
		return GetVFunc(VMI::CDOTABaseAbility::GetAbilityTextureName).Call<const char*>(applyAssetModifiers);
	}

	// JS Func
	int GetEffectiveCastRange() const {
		return GetVFunc(VMI::CDOTABaseAbility::GetEffectiveCastRange).Call<int>(nullptr, nullptr);
	}

};