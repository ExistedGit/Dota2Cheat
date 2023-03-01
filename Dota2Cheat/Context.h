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
	BaseNpc* assignedHero{};
	
	HANDLE CurProcHandle;
	int CurProcId;
	bool IsInMatch = false;

	std::set<ItemRune*> runes{};
	std::set<BaseEntity*> entities{};
	std::set<DotaPlayer*> players{};
	std::set<BaseEntity*> physicalItems{};
	std::set<BaseNpcHero*> heroes{};
	
	struct {
		BaseAbility* midas{};
	} importantItems{};
	sol::state lua{};
};

inline Context ctx{};