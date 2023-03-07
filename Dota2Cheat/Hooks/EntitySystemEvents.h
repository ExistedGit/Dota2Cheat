#pragma once
#include "../SDK/include.h"
#include "VMT.h"

namespace Hooks {
	// Here we filter entities and put them into their respective collections
	CBaseEntity* OnAddEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle);;

	CBaseEntity* OnRemoveEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle);
}