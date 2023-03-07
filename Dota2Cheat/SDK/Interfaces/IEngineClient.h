#pragma once
#include "../Base/VClass.h"
#include "sol/sol.hpp"

class IEngineClient :
	public VClass
{
public:
	int GetLocalPlayerSlot();
	bool IsInGame();
	
	static void BindLua(sol::state& lua);
};

