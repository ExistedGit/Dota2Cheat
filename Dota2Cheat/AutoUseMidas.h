#pragma once

#include "Wrappers.h"

//this one currently returns the handle of midas if it can be used
//since the actual usage is dependent on the entity iteration in dllmain
inline ENT_HANDLE AutoUseMidasCheck(BaseNpc* hero) {
	ENT_HANDLE canUseMidas = 0xFFFFFFFF;
	auto item = hero->FindItemBySubstring("midas");
	if (item.handle != -1 && item.GetAs<BaseAbility>()->GetCooldown() == 0)
		canUseMidas = item.handle;
	return canUseMidas;
}