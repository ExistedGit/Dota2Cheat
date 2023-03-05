#pragma once
#include <Windows.h>
#include <set>
#include <map>
#include <sol/sol.hpp>

class CDOTABaseNPC;
class CBaseEntity;
class CDOTABaseNPC_Hero;
class CDOTAPlayerController;
class CDOTABaseAbility;
class CDOTAItemRune;

struct Context {
	CDOTAPlayerController* localPlayer{};
	CDOTABaseNPC_Hero* assignedHero{};

	HANDLE CurProcHandle;
	int CurProcId;
	enum class GameStage {
		NONE = 0,
		IN_MATCH = 1,
		IN_GAME = 2
	};
	GameStage gameStage = GameStage::NONE;

	std::set<CDOTAItemRune*> runes{};
	std::set<CBaseEntity*> entities{};
	std::set<CDOTAPlayerController*> players{};
	std::set<CBaseEntity*> physicalItems{};
	std::set<CDOTABaseNPC_Hero*> heroes{};

	struct ImportantItems {
		CDOTABaseAbility
			*midas{},
			*manta{},
			*bottle{},
			*armlet{},
			*power_treads{},
			*vambrace;
	} importantItems{};
	sol::state lua{};
};

inline Context ctx{};