#pragma once
#include <Windows.h>

class BaseNpc;
class DotaPlayer;

struct Context {
	DotaPlayer* localPlayer{};
	BaseNpc* assignedHero{};
	HANDLE CurProcHandle;
	int CurProcId;
};

Context ctx{};