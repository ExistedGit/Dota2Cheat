#pragma once
#include <Windows.h>
#include <vector>

class BaseNpc;
class BaseEntity;
class DotaPlayer;

struct Context {
	DotaPlayer* localPlayer{};
	BaseNpc* assignedHero{};
	
	HANDLE CurProcHandle;
	int CurProcId;
	bool IsInMatch = false;
	std::vector<DotaPlayer*> players{};
	std::vector<BaseEntity*> physicalItems{};
	std::vector<BaseNpc*> heroes{};
};

inline Context ctx{};