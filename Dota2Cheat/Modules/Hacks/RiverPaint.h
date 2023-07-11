#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"
#include "../MListeners.h"
namespace Modules {
	// River paint is controlled by a hidden ConVar
	inline class M_RiverPaint: public  IRunFrameListener{
	public:
		void OnFrame() override {
			static auto cvar = CVarSystem::CVars["dota_river_type"];
			cvar.m_pVar->value.ui32 = Config::Changer::RiverListIdx;
		}
	} RiverPaint{};
}