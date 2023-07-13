#pragma once
#include <set>
#include "../../SDK/Entities/CDOTAPlayerController.h"
#include "../../SDK/Entities/CDOTABaseNPCHero.h"

// Context of an ongoing Dota game

enum class GameStage {
	NONE = 0, // In menu
	PRE_GAME = 1, // Joined the match but the game itself didn't start yet
	IN_GAME = 2 // In the game proper
};

inline struct GameContext {
	CDOTAPlayerController* localPlayer{};
	CDOTABaseNPC_Hero* localHero{};

	GameStage gameStage = GameStage::NONE;
} ctx{};