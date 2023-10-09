#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"

#include "../MListeners.h"
#include "../Utility/ParticleGC.h"

namespace Modules {
	inline class M_TargetedSpellHighlighter : public IFrameListener { 
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
		std::unordered_map<std::string, std::function<bool(CDOTAModifier*)>> AdditionalChecks = {
			{
				"modifier_life_stealer_infest_effect",
				[](CDOTAModifier* buff) {
					// We ignore LS infesting allied heroes, as the particle is created on its own
					return !buff->IsSameTeam(ctx.localHero);
				}
			}
		};
		std::unordered_map<std::string, ParticleCreationInfo> ModifierParticles = {
			{
				"modifier_spirit_breaker_charge_of_darkness_vision",
				ParticleCreationInfo(
					"particles/units/heroes/hero_spirit_breaker/spirit_breaker_charge_target.vpcf"
				)
				.SetControlPoint(0, Vector::Zero)
				.SetControlPoint(1, Vector::Zero)
			},
			{
				"modifier_life_stealer_infest_effect",
				ParticleCreationInfo(
					"particles/econ/items/lifestealer/ls_ti10_immortal/ls_ti10_immortal_infest_icon_glow.vpcf"
				)
				.SetControlPoint(0, Vector::Zero)
				.SetControlPoint(1, Vector::Zero)
			},
			{
				"modifier_tusk_snowball_target",
				ParticleCreationInfo(
					"particles/units/heroes/hero_tusk/tusk_snowball_target.vpcf"
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
					"particles/econ/items/bounty_hunter/bounty_hunter_hunters_hoard/bounty_hunter_hoard_shield.vpcf"
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

		void OnFrame() override;

		void OnModifierRemoved(CDOTAModifier* modifier);

		void OnModifierAdded(CDOTAModifier* modifier);

	} TargetedSpellHighlighter{};
}