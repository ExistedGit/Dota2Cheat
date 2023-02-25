#pragma once
#include <Windows.h>
#include <set>
#include <map>
#include <sol/sol.hpp>

class BaseNpc;
class BaseEntity;
class DotaPlayer;
class BaseAbility;

struct Context {
	DotaPlayer* localPlayer{};
	BaseNpc* assignedHero{};
	
	HANDLE CurProcHandle;
	int CurProcId;
	bool IsInMatch = false;

	std::set<BaseEntity*> entities{};
	std::set<DotaPlayer*> players{};
	std::set<BaseEntity*> physicalItems{};
	std::set<BaseNpc*> heroes{};
	struct {
		BaseAbility* midas{};
	} importantItems{};
	sol::state lua{};
};

inline Context ctx{};