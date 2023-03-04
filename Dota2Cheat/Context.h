#pragma once
#include <Windows.h>
#include <set>
#include <map>
#include <sol/sol.hpp>

class BaseNpc;
class BaseEntity;
class BaseNpcHero;
class DotaPlayer;
class BaseAbility;
class ItemRune;

struct Context {
	DotaPlayer* localPlayer{};
	BaseNpcHero* assignedHero{};
	
	HANDLE CurProcHandle;
	int CurProcId;
	enum class GameStage {
		NONE = 0,
		IN_MATCH = 1,
		IN_GAME = 2
	};
	GameStage gameStage = GameStage::NONE;

	std::set<ItemRune*> runes{};
	std::set<BaseEntity*> entities{};
	std::set<DotaPlayer*> players{};
	std::set<BaseEntity*> physicalItems{};
	std::set<BaseNpcHero*> heroes{};
	
	struct ImportantItems {
		BaseAbility* midas{};
		BaseAbility* manta{};
		BaseAbility* bottle{};
	} importantItems{};
	sol::state lua{};
};

inline Context ctx{};