#pragma once
#include "../SDK/pch.h"

namespace Hooks {
	typedef CBaseEntity* (__fastcall* EntSystemEvent)(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle);
	inline EntSystemEvent oOnAddEntity{};
	inline EntSystemEvent oOnRemoveEntity{};

	// Here we filter entities and put them into their respective collections
	CBaseEntity* OnAddEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle);

	CBaseEntity* OnRemoveEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle);
}