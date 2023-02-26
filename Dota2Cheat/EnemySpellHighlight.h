#pragma once
#include "sdk.h"
#include "Globals.h"
#include "ParticleGC.h"

namespace Hacks {
	class EnemySpellHighlighter {
	private:
		struct ParticleCreationInfo {
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
		};

		std::map<std::string_view, ParticleCreationInfo> ModifierParticles = {
			{
				"modifier_invoker_sun_strike",
				ParticleCreationInfo(
					"particles/econ/items/invoker/invoker_apex/invoker_sun_strike_team_immortal1.vpcf"
				).SetControlPoint(1, Vector3::Zero)
			},
			{
				"modifier_kunkka_torrent_thinker",
				ParticleCreationInfo(
					"particles/units/heroes/hero_kunkka/kunkka_spell_torrent_bubbles.vpcf"
				).SetDieTime(2)
			},
			{
				"modifier_lina_light_strike_array",
				ParticleCreationInfo(
					"particles/units/heroes/hero_lina/lina_spell_light_strike_array_ray_team.vpcf"
				).SetControlPoint(1, Vector3(250, 1, 1))
			}
		};
	public:
		void DrawParticleAt(Vector3 pos, ParticleCreationInfo info) {
			auto particleWrap = Globals::ParticleManager->CreateParticle(
				info.particleName,
				PATTACH_WORLDORIGIN,
				nullptr);
			particleWrap.particle
				->SetControlPoint(0, &pos);
			for (auto& [idx, val] : info.controlPoints)
				particleWrap.particle->SetControlPoint(idx, &val);

			if (info.dieTime)
				Modules::ParticleGC.SetDieTime(particleWrap, info.dieTime);
		}

		void RenderIfThinkerModifier(DotaModifier* modifier) {
			if (!Config::ShowEnemyPointSpells)
				return;

			if (ModifierParticles.count(modifier->GetName())) {
				auto thinker = modifier->GetOwner();
				if (thinker->GetTeam() != ctx.assignedHero->GetTeam()) {

					DrawParticleAt(thinker->GetPos(), ModifierParticles[modifier->GetName()]);
				}
			}
		}
	};
}
namespace Modules {
	inline Hacks::EnemySpellHighlighter EnemySpellHighlighter{};
}