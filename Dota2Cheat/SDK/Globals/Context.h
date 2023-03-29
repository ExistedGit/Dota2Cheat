#pragma once
#include <Windows.h>
#include <set>
#include <string>
#include <sol/sol.hpp>

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
		NONE = 0,
		IN_MATCH = 1,
		IN_GAME = 2
	};
	GameStage gameStage = GameStage::NONE;

	std::set<CDOTAItemRune*> runes;
	std::set<CBaseEntity*> entities;
	std::set<CDOTAPlayerController*> players;
	std::set<CBaseEntity*> physicalItems;
	std::set<CDOTABaseNPC_Hero*> heroes;

	struct ImportantItems {
		CDOTAItem
			*midas{},
			*manta{},
			*bottle{},
			*armlet{},
			*power_treads{},
			*vambrace;
	} importantItems{};

	std::string cheatFolderPath;
	sol::state lua{};

	// For Lua compatibility
	Context() {};
	Context(const Context& other) { };
};

inline Context ctx{};