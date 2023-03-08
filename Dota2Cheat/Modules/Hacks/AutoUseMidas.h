#pragma once
#include "../../SDK/pch.h"
#include "../../Config.h"

inline bool CanUseMidas() {
	if (!Config::AutoMidasEnabled)
		return false;

	auto midas = ctx.importantItems.midas;
	return midas && midas->GetCooldown() == 0;
}