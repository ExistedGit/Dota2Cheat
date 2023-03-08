#pragma once
#include <vector>
#include "../../SDK/pch.h"
#include "../Utility/ParticleGC.h"
#include "../../Config.h"


namespace Hacks {
	class TargetedSpellHighlighter {
	private:
		struct ParticleCreationInfo {
			std::map<int, Vector> controlPoints{};
			float dieTime{};
			const char* particleName{};
			ParticleCreationInfo() {}
			ParticleCreationInfo(const char* particleName) : particleName(particleName) {

			}
			ParticleCreationInfo& SetControlPoint(int idx, const Vector& vec) {
				controlPoints[idx] = vec;
				return *this;
			}
			ParticleCreationInfo& SetDieTime(float dieTime) {
				this->dieTime = dieTime;
				return *this;
			}
		};

		std::map<std::string_view, ParticleCreationInfo> ModifierParticles = {
			{
				"modifier_spirit_breaker_charge_of_darkness_vision",
				ParticleCreationInfo(
					"particles/units/heroes/hero_spirit_breaker/spirit_breaker_charge_target.vpcf"
				)
				.SetControlPoint(0, Vector::Zero)
				.SetControlPoint(1, Vector::Zero)
			},
			{
				"modifier_sniper_assassinate",
				ParticleCreationInfo(
					"particles/units/heroes/hero_sniper/sniper_crosshair.vpcf"
				)
				.SetControlPoint(0, Vector::Zero)
				.SetControlPoint(60, Vector::Zero) // RGB color of the crosshair
				.SetControlPoint(61, Vector::Zero) // X coordinate controls whether the color applies
			},
			{
				"modifier_bounty_hunter_track",
				ParticleCreationInfo(
					"particles/units/heroes/hero_bounty_hunter/bounty_hunter_track_shield.vpcf"
				)
				.SetControlPoint(0, Vector::Zero)
				.SetControlPoint(3, Vector::Zero)
			}
		};

		// Separate structure due to Linken's Sphere being tracked via its cooldown
		std::map<CBaseEntity*, ParticleWrapper> LinkenSphereParticles{};
		// Map of heroes to their linken sphere items
		std::map<CBaseEntity*, CDOTABaseAbility*> HeroesWithLinken{};

		std::map<CDOTAModifier*, ParticleWrapper> TrackedModifiers{};
	public:
		void Reset();

		void OnDisableTargetedSpells();

		void OnDisableLinken();

		void SubscribeLinkenRendering(CBaseEntity* ent, CDOTABaseAbility* sphere);
		void UnsubscribeLinkenRendering(CBaseEntity* ent);

		void RemoveLinkenEffectFor(CBaseEntity* ent);

		void DrawLinkenEffectFor(CBaseEntity* ent);

		void FrameBasedLogic();

		void RemoveParticleIfTargetedSpell(CDOTAModifier* modifier);

		void DrawParticleIfTargetedSpell(CDOTAModifier* modifier);

	};
}

namespace Modules {
	inline Hacks::TargetedSpellHighlighter TargetedSpellHighlighter{};
}