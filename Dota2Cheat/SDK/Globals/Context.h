#pragma once
#include <Windows.h>
#include <set>
#include <string>

// Context of an ongoing Dota game

class CBaseEntity;
class CDOTABaseNPC;
class CDOTABaseNPC_Hero;
class CDOTAPlayerController;
class CDOTABaseAbility;
class CDOTAItem;
class CDOTAItemRune;

enum class GameStage {
	NONE = 0, // In menu
	PRE_GAME = 1, // Joined the match but the game itself didn't start yet
	IN_GAME = 2 // In the game proper
};

inline struct GameContext {
	CDOTAPlayerController* localPlayer{};
	CDOTABaseNPC_Hero* localHero{};

	GameStage gameStage = GameStage::NONE;

	std::set<CDOTAItemRune*> runes;
	std::set<CBaseEntity*> entities;
	std::set<CDOTAPlayerController*> players;
	std::set<CDOTABaseNPC_Hero*> heroes;
} ctx{};