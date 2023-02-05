#pragma once
#include "sdk.h"
#include "Globals.h"

namespace Hacks::SunStrikeHighlighter {

	inline BaseEntity* SunStrikeThinker = nullptr;
	inline bool SunStrikeIncoming = false;
	inline void DrawEnemySunstrike(Vector3 pos) {
		auto particle = Globals::ParticleManager->CreateParticle(
			"particles/econ/items/invoker/invoker_apex/invoker_sun_strike_team_immortal1.vpcf",
			CDOTAParticleManager::ParticleAttachment_t::PATTACH_WORLDORIGIN,
			nullptr).particle;
		particle->SetControlPoint(0, &pos)
			->SetControlPoint(1, &Vector3::Zero);
	}
}