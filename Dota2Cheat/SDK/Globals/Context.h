#pragma once
#include <Windows.h>
#include <set>
#include <string>
#include <sol/sol.hpp>
#include "../Wrappers/Creep.h"

class CBaseEntity;
class CDOTABaseNPC;
class CDOTABaseNPC_Hero;
class CDOTAPlayerController;
class CDOTABaseAbility;
class CDOTAItem;
class CDOTAItemRune;

struct Context {
	CDOTAPlayerController* localPlayer{};
	CDOTABaseNPC_Hero* assignedHero{};
	uint32_t assignedHeroHandle{};

	HANDLE CurProcHandle;
	int CurProcId;
	enum class GameStage {
		NONE = 0, // In menu
		PRE_GAME = 1, // Joined the match but the game itself didn't start yet
		IN_GAME = 2 // In the game proper
	};
	GameStage gameStage = GameStage::NONE;

	std::set<CDOTAItemRune*> runes;
	std::set<CBaseEntity*> entities;
	std::set<CDOTAPlayerController*> players;
	std::set<CBaseEntity*> physicalItems;
	std::set<CDOTABaseNPC_Hero*> heroes;
	std::set<CreepWrapper, CreepWrapper_less> creeps;

	std::map<std::string, CDOTAItem*> ImportantItems;

	std::string cheatFolderPath;
	sol::state lua;

	// For Lua compatibility
	Context() {};
	Context(const Context& other) { };
};

inline Context ctx{};