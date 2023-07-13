#pragma once
#include "../../pch.h"
#include "../../CheatSDK/Config.h"
#include "../Utility/ParticleGC.h"

namespace Modules {
inline 
	class EnemySpellHighlighter {
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
				"modifier_invoker_sun_strike",
				ParticleCreationInfo(
					"particles/econ/items/invoker/invoker_apex/invoker_sun_strike_team_immortal1.vpcf"
				).SetControlPoint(1, Vector::Zero)
			},
			{
				"modifier_kunkka_torrent_thinker",
				ParticleCreationInfo(
					"particles/econ/items/kunkka/divine_anchor/hero_kunkka_dafx_skills/kunkka_spell_torrent_bubbles_fxset.vpcf"
				).SetDieTime(2)
			},
			{
				"modifier_lina_light_strike_array",
				ParticleCreationInfo(
					"particles/econ/items/lina/lina_ti7/light_strike_array_pre_ti7.vpcf"
				).SetControlPoint(1, Vector(250, 1, 1))
			}
		};
	public:
		void DrawParticleAt(Vector pos, ParticleCreationInfo info);

		void OnModifierAdded(CDOTAModifier* modifier);
	} EnemySpellHighlighter;
}