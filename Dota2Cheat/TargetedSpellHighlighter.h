#pragma once
#include <vector>
#include "CUtlVector.h"
#include "Wrappers.h"
#include "Globals.h"
#include "ParticleGC.h"


namespace Hacks {
	class TargetedSpellHighlighter {
	private:
		struct ParticleCreationInfo {
			ParticleAttachment_t attachType;

			std::map<int, Vector3> controlPoints{};
			float dieTime{};
			const char* particleName{};
			ParticleCreationInfo() {}
			ParticleCreationInfo(const char* particleName) : particleName(particleName) {

			}
			ParticleCreationInfo& SetControlPoint(int idx, const Vector3& vec) {
				controlPoints[idx] = vec;
				return *this;
			}
			ParticleCreationInfo& SetDieTime(float dieTime) {
				this->dieTime = dieTime;
				return *this;
			}
			ParticleCreationInfo& SetAttachType(ParticleAttachment_t value) {
				attachType = value;
				return *this;
			}
		};

		std::map<std::string_view, ParticleCreationInfo> ModifierParticles = {
			{
				"modifier_spirit_breaker_charge_of_darkness_vision",
				ParticleCreationInfo(
					"particles/units/heroes/hero_spirit_breaker/spirit_breaker_charge_target.vpcf"
				)
				.SetControlPoint(0, Vector3::Zero)
				.SetControlPoint(1, Vector3::Zero)
				.SetAttachType(PATTACH_OVERHEAD_FOLLOW)
			},
			{
				"modifier_sniper_assassinate",
				ParticleCreationInfo(
					"particles/units/heroes/hero_sniper/sniper_crosshair.vpcf"
				)
				.SetControlPoint(0, Vector3::Zero)
				.SetControlPoint(60, Vector3::Zero) // RGB color of the crosshair
				.SetControlPoint(61, Vector3::Zero) // X coordinate controls whether the color applies
				.SetAttachType(PATTACH_OVERHEAD_FOLLOW)
			}
		};

		// Separate structure due to Linken's Sphere being tracked via its cooldown
		std::map<BaseEntity*, CDOTAParticleManager::ParticleWrapper> LinkenSphereParticles{};
		// Map of heroes to their linken sphere items
		std::map<BaseEntity*, BaseAbility*> HeroesWithLinken{};

		std::map<DotaModifier*, CDOTAParticleManager::ParticleWrapper> TrackedModifiers{};
	public:
		void Reset() {
			HeroesWithLinken.clear();
			TrackedModifiers.clear();
			LinkenSphereParticles.clear();
		}

		void OnDisableTargetedSpells() {
			for (auto& [_, pw] : TrackedModifiers)
				Globals::ParticleManager->DestroyParticle(pw);

			TrackedModifiers.clear();
		}

		void OnDisableLinken() {
			for (auto& [hero, _] : HeroesWithLinken)
				Globals::ParticleManager->DestroyParticle(LinkenSphereParticles[hero]);

			LinkenSphereParticles.clear();
		}

		void SubscribeLinkenRendering(BaseEntity* ent, BaseAbility* sphere) {
			HeroesWithLinken[ent] = sphere;
		}
		void UnsubscribeLinkenRendering(BaseEntity* ent) {
			RemoveLinkenEffectFor(ent);
			HeroesWithLinken.erase(ent);
		}

		void RemoveLinkenEffectFor(BaseEntity* ent) {
			if (!LinkenSphereParticles.count(ent))
				return;

			Globals::ParticleManager->DestroyParticle(LinkenSphereParticles[ent]);
			LinkenSphereParticles.erase(ent);
		}

		void DrawLinkenEffectFor(BaseEntity* ent) {
			if (!Config::ShowLinkenSphere)
				return;

			if (LinkenSphereParticles.count(ent))
				return;

			LinkenSphereParticles[ent] = Globals::ParticleManager->CreateParticle(
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

		void RemoveParticleIfTargetedSpell(DotaModifier* modifier) {
			if (!TrackedModifiers.count(modifier))
				return;
			Globals::ParticleManager->DestroyParticle(TrackedModifiers[modifier]);
			TrackedModifiers.erase(modifier);
		}

		void DrawParticleIfTargetedSpell(DotaModifier* modifier) {
			if (!Config::ShowEnemyTargetedSpells)
				return;
			if (!ModifierParticles.count(modifier->GetName()))
				return;

			auto entry = ModifierParticles[modifier->GetName()];

			TrackedModifiers[modifier] = Globals::ParticleManager->CreateParticle(
				entry.particleName,
				entry.attachType,
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