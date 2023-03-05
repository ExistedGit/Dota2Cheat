#pragma once
#include <vector>
#include "../../SDK/include.h"
#include "../Utility/ParticleGC.h"


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
		void Reset() {
			OnDisableTargetedSpells();
			OnDisableLinken();
			HeroesWithLinken.clear();
		}

		void OnDisableTargetedSpells() {
			for (auto& [_, pw] : TrackedModifiers)
				if (pw.handle != 0xFFFFFFFF)
					GameSystems::ParticleManager->DestroyParticle(pw);
			TrackedModifiers.clear();
		}

		void OnDisableLinken() {
			for (auto& [hero, _] : HeroesWithLinken)
				GameSystems::ParticleManager->DestroyParticle(LinkenSphereParticles[hero]);

			LinkenSphereParticles.clear();
		}

		void SubscribeLinkenRendering(CBaseEntity* ent, CDOTABaseAbility* sphere) {
			HeroesWithLinken[ent] = sphere;
		}
		void UnsubscribeLinkenRendering(CBaseEntity* ent) {
			RemoveLinkenEffectFor(ent);
			HeroesWithLinken.erase(ent);
		}

		void RemoveLinkenEffectFor(CBaseEntity* ent) {
			if (!LinkenSphereParticles.count(ent))
				return;

			GameSystems::ParticleManager->DestroyParticle(LinkenSphereParticles[ent]);
			LinkenSphereParticles.erase(ent);
		}

		void DrawLinkenEffectFor(CBaseEntity* ent) {
			if (!Config::ShowLinkenSphere)
				return;

			if (LinkenSphereParticles.count(ent))
				return;

			LinkenSphereParticles[ent] = GameSystems::ParticleManager->CreateParticle(
				"particles/items_fx/immunity_sphere_buff.vpcf",
				PATTACH_ROOTBONE_FOLLOW,
				ent
			);

		}

		void FrameBasedLogic() {

			for (auto& [hero, ability] : HeroesWithLinken) {
				if (!hero->GetIdentity()->IsDormant() && ability->GetCooldown() == 0)
					DrawLinkenEffectFor(hero);
				else
					RemoveLinkenEffectFor(hero);
			}
		}

		void RemoveParticleIfTargetedSpell(CDOTAModifier* modifier) {
			if (!TrackedModifiers.count(modifier))
				return;
			GameSystems::ParticleManager->DestroyParticle(TrackedModifiers[modifier]);
			TrackedModifiers.erase(modifier);
		}

		void DrawParticleIfTargetedSpell(CDOTAModifier* modifier) {
			if (!Config::ShowEnemyTargetedSpells)
				return;
			if (!ModifierParticles.count(modifier->GetName()))
				return;
			if (modifier->GetOwner()->GetTeam() != ctx.assignedHero->GetTeam())
				return;

			auto entry = ModifierParticles[modifier->GetName()];

			TrackedModifiers[modifier] = GameSystems::ParticleManager->CreateParticle(
				entry.particleName,
				PATTACH_OVERHEAD_FOLLOW,
				modifier->GetOwner()
			);
			for (auto& cp : entry.controlPoints)
				TrackedModifiers[modifier].particle
				->SetControlPoint(cp.first, &cp.second);

			if (entry.dieTime)
				Modules::ParticleGC.SetDieTime(TrackedModifiers[modifier], entry.dieTime);

		}

	};
}

namespace Modules {
	inline Hacks::TargetedSpellHighlighter TargetedSpellHighlighter{};
}