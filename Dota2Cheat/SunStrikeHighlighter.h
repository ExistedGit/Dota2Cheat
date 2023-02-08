#pragma once
#include "sdk.h"
#include "Globals.h"

namespace Hacks {
	class SunStrikeHighlighter {
	private:
		BaseEntity* SunStrikeThinker = nullptr;
	public:
		bool SunStrikeIncoming = false;

		// Also sets SunStrikeIncoming to false
		inline void QueueThinker(BaseEntity* thinker) {
			SunStrikeThinker = thinker;
			SunStrikeIncoming = false;
		}

		inline void FrameBasedLogic() {
			if (SunStrikeThinker != nullptr
				&& SunStrikeThinker->GetPos() != Vector3::Zero) {
				DrawEnemySunstrike(SunStrikeThinker->GetPos());
				SunStrikeThinker = nullptr;
			}
		}

		inline void DrawEnemySunstrike(Vector3 pos) {
			Globals::ParticleManager->CreateParticle(
				"particles/econ/items/invoker/invoker_apex/invoker_sun_strike_team_immortal1.vpcf",
				CDOTAParticleManager::ParticleAttachment_t::PATTACH_WORLDORIGIN,
				nullptr).particle
				->SetControlPoint(0, &pos)
				->SetControlPoint(1, &Vector3::Zero);
		}
	};
}
namespace Modules {
	inline Hacks::SunStrikeHighlighter SunStrikeHighlighter{};
}