#pragma once

enum DotaUnitType : int {
	DOTA_UNIT_NONE = 0,
	DOTA_UNIT_HERO,
	DOTA_UNIT_STRUCTURE,
	DOTA_UNIT_TOWER,
	DOTA_UNIT_ANCIENT,
	DOTA_UNIT_BARRACKS,
	DOTA_UNIT_CREEP,
	DOTA_UNIT_COURIER,
	DOTA_UNIT_LANE_CREEP,
	DOTA_UNIT_ROSHAN,
	DOTA_UNIT_SHOPKEEP
};

enum class ModifierState : int {
	MODIFIER_STATE_ROOTED = 0,
	MODIFIER_STATE_DISARMED = 1,
	MODIFIER_STATE_ATTACK_IMMUNE = 2,
	MODIFIER_STATE_SILENCED = 3,
	MODIFIER_STATE_MUTED = 4,
	MODIFIER_STATE_STUNNED = 5,
	MODIFIER_STATE_HEXED = 6,
	MODIFIER_STATE_INVISIBLE = 7,
	MODIFIER_STATE_INVULNERABLE = 8,
	MODIFIER_STATE_MAGIC_IMMUNE = 9,
	MODIFIER_STATE_PROVIDES_VISION = 10,
	MODIFIER_STATE_NIGHTMARED = 11,
	MODIFIER_STATE_BLOCK_DISABLED = 12,
	MODIFIER_STATE_EVADE_DISABLED = 13,
	MODIFIER_STATE_UNSELECTABLE = 14,
	MODIFIER_STATE_CANNOT_TARGET_ENEMIES = 15,
	MODIFIER_STATE_CANNOT_MISS = 16,
	MODIFIER_STATE_SPECIALLY_DENIABLE = 17,
	MODIFIER_STATE_FROZEN = 18,
	MODIFIER_STATE_COMMAND_RESTRICTED = 19,
	MODIFIER_STATE_NOT_ON_MINIMAP = 20,
	MODIFIER_STATE_LOW_ATTACK_PRIORITY = 21,
	MODIFIER_STATE_NO_HEALTH_BAR = 22,
	MODIFIER_STATE_NO_HEALTH_BAR_FOR_ENEMIES = 23,
	MODIFIER_STATE_FLYING = 24,
	MODIFIER_STATE_NO_UNIT_COLLISION = 25,
	MODIFIER_STATE_NO_TEAM_MOVE_TO = 26,
	MODIFIER_STATE_NO_TEAM_SELECT = 27,
	MODIFIER_STATE_PASSIVES_DISABLED = 28,
	MODIFIER_STATE_DOMINATED = 29,
	MODIFIER_STATE_BLIND = 30,
	MODIFIER_STATE_OUT_OF_GAME = 31,
	MODIFIER_STATE_FAKE_ALLY = 32,
	MODIFIER_STATE_FLYING_FOR_PATHING_PURPOSES_ONLY = 33,
	MODIFIER_STATE_TRUESIGHT_IMMUNE = 34,
	MODIFIER_STATE_UNTARGETABLE = 35,
	MODIFIER_STATE_IGNORING_MOVE_AND_ATTACK_ORDERS = 36,
	MODIFIER_STATE_ALLOW_PATHING_THROUGH_TREES = 37,
	MODIFIER_STATE_NOT_ON_MINIMAP_FOR_ENEMIES = 38,
	MODIFIER_STATE_UNSLOWABLE = 39,
	MODIFIER_STATE_TETHERED = 40,
	MODIFIER_STATE_IGNORING_STOP_ORDERS = 41,
	MODIFIER_STATE_FEARED = 42,
	MODIFIER_STATE_TAUNTED = 43,
	MODIFIER_STATE_CANNOT_BE_MOTION_CONTROLLED = 44,
	MODIFIER_STATE_FORCED_FLYING_VISION = 45,
	MODIFIER_STATE_ATTACK_ALLIES = 46,
	MODIFIER_STATE_ALLOW_PATHING_THROUGH_CLIFFS = 47,
	MODIFIER_STATE_ALLOW_PATHING_THROUGH_FISSURE = 48,
	MODIFIER_STATE_SPECIALLY_UNDENIABLE = 49,
	MODIFIER_STATE_ALLOW_PATHING_THROUGH_OBSTRUCTIONS = 50,
	MODIFIER_STATE_LAST = 51
};

enum class AttackCapabilities {
	MELEE = 1,
	RANGED
};

enum DOTA_HeroPickState : int {
	DOTA_HEROPICK_STATE_NONE = 0,
	DOTA_HEROPICK_STATE_AP_SELECT = 1,
	DOTA_HEROPICK_STATE_SD_SELECT = 2,
	DOTA_HEROPICK_STATE_INTRO_SELECT_UNUSED = 3,
	DOTA_HEROPICK_STATE_RD_SELECT_UNUSED = 4,
	DOTA_HEROPICK_STATE_CM_INTRO = 5,
	DOTA_HEROPICK_STATE_CM_CAPTAINPICK = 6,
	DOTA_HEROPICK_STATE_CM_BAN1 = 7,
	DOTA_HEROPICK_STATE_CM_BAN2 = 8,
	DOTA_HEROPICK_STATE_CM_BAN3 = 9,
	DOTA_HEROPICK_STATE_CM_BAN4 = 10,
	DOTA_HEROPICK_STATE_CM_BAN5 = 11,
	DOTA_HEROPICK_STATE_CM_BAN6 = 12,
	DOTA_HEROPICK_STATE_CM_BAN7 = 13,
	DOTA_HEROPICK_STATE_CM_BAN8 = 14,
	DOTA_HEROPICK_STATE_CM_BAN9 = 15,
	DOTA_HEROPICK_STATE_CM_BAN10 = 16,
	DOTA_HEROPICK_STATE_CM_BAN11 = 17,
	DOTA_HEROPICK_STATE_CM_BAN12 = 18,
	DOTA_HEROPICK_STATE_CM_BAN13 = 19,
	DOTA_HEROPICK_STATE_CM_BAN14 = 20,
	DOTA_HEROPICK_STATE_CM_SELECT1 = 21,
	DOTA_HEROPICK_STATE_CM_SELECT2 = 22,
	DOTA_HEROPICK_STATE_CM_SELECT3 = 23,
	DOTA_HEROPICK_STATE_CM_SELECT4 = 24,
	DOTA_HEROPICK_STATE_CM_SELECT5 = 25,
	DOTA_HEROPICK_STATE_CM_SELECT6 = 26,
	DOTA_HEROPICK_STATE_CM_SELECT7 = 27,
	DOTA_HEROPICK_STATE_CM_SELECT8 = 28,
	DOTA_HEROPICK_STATE_CM_SELECT9 = 29,
	DOTA_HEROPICK_STATE_CM_SELECT10 = 30,
	DOTA_HEROPICK_STATE_CM_PICK = 31,
	DOTA_HEROPICK_STATE_AR_SELECT = 32,
	DOTA_HEROPICK_STATE_MO_SELECT = 33,
	DOTA_HEROPICK_STATE_FH_SELECT = 34,
	DOTA_HEROPICK_STATE_CD_INTRO = 35,
	DOTA_HEROPICK_STATE_CD_CAPTAINPICK = 36,
	DOTA_HEROPICK_STATE_CD_BAN1 = 37,
	DOTA_HEROPICK_STATE_CD_BAN2 = 38,
	DOTA_HEROPICK_STATE_CD_BAN3 = 39,
	DOTA_HEROPICK_STATE_CD_BAN4 = 40,
	DOTA_HEROPICK_STATE_CD_BAN5 = 41,
	DOTA_HEROPICK_STATE_CD_BAN6 = 42,
	DOTA_HEROPICK_STATE_CD_SELECT1 = 43,
	DOTA_HEROPICK_STATE_CD_SELECT2 = 44,
	DOTA_HEROPICK_STATE_CD_SELECT3 = 45,
	DOTA_HEROPICK_STATE_CD_SELECT4 = 46,
	DOTA_HEROPICK_STATE_CD_SELECT5 = 47,
	DOTA_HEROPICK_STATE_CD_SELECT6 = 48,
	DOTA_HEROPICK_STATE_CD_SELECT7 = 49,
	DOTA_HEROPICK_STATE_CD_SELECT8 = 50,
	DOTA_HEROPICK_STATE_CD_SELECT9 = 51,
	DOTA_HEROPICK_STATE_CD_SELECT10 = 52,
	DOTA_HEROPICK_STATE_CD_PICK = 53,
	DOTA_HEROPICK_STATE_BD_SELECT = 54,
	DOTA_HERO_PICK_STATE_ABILITY_DRAFT_SELECT = 55,
	DOTA_HERO_PICK_STATE_ARDM_SELECT = 56,
	DOTA_HEROPICK_STATE_ALL_DRAFT_SELECT = 57,
	DOTA_HERO_PICK_STATE_CUSTOMGAME_SELECT = 58,
	DOTA_HEROPICK_STATE_SELECT_PENALTY = 59,
	DOTA_HEROPICK_STATE_CUSTOM_PICK_RULES = 60,
	DOTA_HEROPICK_STATE_SCENARIO_PICK = 61,
	DOTA_HEROPICK_STATE_COUNT = 62
};

enum class DotaRunes : int {
	INVALID = -1,
	DOUBLEDAMAGE = 0,
	HASTE = 1,
	ILLUSION = 2,
	INVISIBILITY = 3,
	REGENERATION = 4,
	BOUNTY = 5,
	COUNT = 6, //???
	WATER = 7
};

//what units will receive the PrepareUnitOrders() command
enum PlayerOrderIssuer_t : int {
	DOTA_ORDER_ISSUER_SELECTED_UNITS = 0,
	DOTA_ORDER_ISSUER_CURRENT_UNIT_ONLY = 1,
	DOTA_ORDER_ISSUER_HERO_ONLY = 2,
	DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY = 3
};
