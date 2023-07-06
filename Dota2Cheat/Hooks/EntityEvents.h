#pragma once
#include "../SDK/pch.h"
#include "../CheatSDK/include.h"
#include "../Modules/Hacks/AegisSnatcher.h"
#include "../SDK/Wrappers/EntitySorting.h"

namespace Hooks {
	// CGameSceneNode::SetAbsOrigin
	typedef void* (__fastcall* SetAbsOriginFn)(VClass* thisptr, bool unk);
	inline SetAbsOriginFn oSetAbsOrigin;
	inline void* hkSetAbsOrigin(VClass* thisptr, bool unk) {
		auto ret = oSetAbsOrigin(thisptr, unk);
		CBaseEntity* ent = thisptr->Member<CBaseEntity*>(0x30);

		//if (ctx.heroes.contains((CDOTABaseNPC_Hero*)ent)) {
		//	auto hero = (CDOTABaseNPC_Hero*)ent;
		//	HeroData[hero].AbsOrigin = hero->GetPos();
		//	HeroData[hero].HealthbarW2S = WorldToScreen(hero->GetHealthBarPos());
		//}

		return ret;
	}
}