#pragma once
#include <map>
#include "../../SDK/include.h"


namespace Hacks {
	class ParticleAbilityWarner {
		// Draws a dashed red line from begin to end
		// Returns the wrapper for the created particle
		auto DrawTrajectory(Vector begin, Vector end) {
			auto pw = GameSystems::ParticleManager->CreateParticle(
				"particles/ui_mouseactions/range_finder_tower_line.vpcf",
				PATTACH_WORLDORIGIN,
				ctx.assignedHero
			);
			Vector boolCp{ 1, 0, 0 };
			pw.particle
				->SetControlPoint(2, &begin)
				->SetControlPoint(6, &boolCp)
				->SetControlPoint(7, &end);
			return pw;
		}

		enum AbilityParticles : uint64_t {
			AP_PUDGE_MEAT_HOOK = 16517413739925325824,
			AP_KOTL_ILLUMINATE = 2498180139172148061,
			AP_BANSHEE_CRYPT_SWARM = 3020948711683823655,
			AP_MEEPO_EARTHBIND = 9143050083230003323
		};

		struct AbilityParticleInfo {
			AbilityParticles nameIndex;
			Vector begin{}, end{};
			CDOTABaseNPC* owner;
		};
		// Map of particle name indexes to their respective ability indexes
		const std::map<AbilityParticles, uint32_t> AbilityIndexes{
			{AP_KOTL_ILLUMINATE, 7},	// illuminate is moved to index 7 when End Illuminate takes its place
			{AP_BANSHEE_CRYPT_SWARM, 0}
		};
		std::map<uint32_t, AbilityParticleInfo> queuedParticleIndexes{};
		std::map<uint32_t, ParticleWrapper> TrackedAbilityParticles{};

		CDOTABaseNPC_Hero* FindParticleOwner(const char* name) {
			for (auto& hero : ctx.heroes)
				if (!strcmp(hero->GetUnitName(), name)) {
					return hero;
					break;
				}
			return nullptr;
		}
	public:
		// Meat hook:
		// On create: begin = fallback_position 
		// Update: end = control point 1

		// Illuminate, Crypt Swarm:
		// 
		// Update: velocity = control point 1

		void ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
			if (msgHandle->messageID == 145) {

				auto pmMsg = reinterpret_cast<CUserMsg_ParticleManager*>(msg);
				auto msgIndex = pmMsg->index();
				switch (pmMsg->type()) {
					// Catches any net-created particles to see if they represent an ability
					// Places them into queuedParticleIndexes until further messages are processed
				case GAME_PARTICLE_MANAGER_EVENT_CREATE: {

					auto particle = pmMsg->create_particle();
					switch (particle.particle_name_index()) {

					case AP_KOTL_ILLUMINATE:
					case AP_PUDGE_MEAT_HOOK:
					case AP_BANSHEE_CRYPT_SWARM:
					case AP_MEEPO_EARTHBIND:
						auto& qParticle = queuedParticleIndexes[msgIndex] = AbilityParticleInfo{
							.nameIndex = (AbilityParticles)particle.particle_name_index()
						};

						if (qParticle.nameIndex == AP_KOTL_ILLUMINATE)
							qParticle.owner = FindParticleOwner("npc_dota_hero_keeper_of_the_light");
						else if (qParticle.nameIndex == AP_BANSHEE_CRYPT_SWARM)
							qParticle.owner = FindParticleOwner("npc_dota_hero_death_prophet");
						else if (qParticle.nameIndex == AP_MEEPO_EARTHBIND)
							qParticle.owner = FindParticleOwner("npc_dota_hero_meepo");


						break;
					}

					break;
				}
													   // Gets the ability owner and determines if the ability trajectory will be drawn
				case GAME_PARTICLE_MANAGER_EVENT_UPDATE_ENT:
				{
					if (!queuedParticleIndexes.count(msgIndex))
						break;

					auto updParticleEnt = pmMsg->update_particle_ent();
					auto owner = Interfaces::EntitySystem->GetEntity(NH2IDX(updParticleEnt.entity_handle()));
					// If they're not an enemy we dequeue the particle's index
					if (!owner || owner->GetTeam() == ctx.assignedHero->GetTeam()) {
						queuedParticleIndexes.erase(msgIndex);
						break;
					}
					auto& info = queuedParticleIndexes[msgIndex];

					switch (info.nameIndex) {
					case AbilityParticles::AP_PUDGE_MEAT_HOOK: {
						info.begin = Vector{
							updParticleEnt.fallback_position().x(),
							updParticleEnt.fallback_position().y(),
							updParticleEnt.fallback_position().z()
						};
						info.owner = (CDOTABaseNPC*)owner;
						break;
					}
					}
				}
				// Here we register the actual trajectory
				case GAME_PARTICLE_MANAGER_EVENT_UPDATE:
				{
					if (!queuedParticleIndexes.count(msgIndex))
						break;


					auto& info = queuedParticleIndexes[msgIndex];
					auto cpIdx = pmMsg->update_particle().control_point();
					auto cpVal = Vector{
							pmMsg->update_particle().position().x(),
							pmMsg->update_particle().position().y(),
							pmMsg->update_particle().position().z()
					};

					switch (info.nameIndex) {
					case AP_PUDGE_MEAT_HOOK:
						// Hook's destination
						if (cpIdx == 1) {
							info.end = cpVal;
							TrackedAbilityParticles[msgIndex] = DrawTrajectory(info.begin, info.end);
							Modules::ParticleGC.SetDieTime(TrackedAbilityParticles[msgIndex], 3);
						}
						break;
					case AP_KOTL_ILLUMINATE:
					case AP_BANSHEE_CRYPT_SWARM:
						if (cpIdx == 0)
							info.begin = cpVal;
						// Particle's velocity
						// Same calculations as in LinearProjectileWarner
						else if (cpIdx == 1) {
							auto castRange =
								info.owner->GetAbilities()[
									AbilityIndexes.at(info.nameIndex)
								]
								.GetEnt()->GetEffectiveCastRange();

							if (info.nameIndex == AP_BANSHEE_CRYPT_SWARM)
								castRange += 390; // hits past its cast range, for some reason

							auto ratio = castRange / cpVal.Length2D();
							auto enlargedVec = cpVal.To<Vector2D>() * ratio;
							info.end = info.begin;
							info.end.x += enlargedVec.x;
							info.end.y += enlargedVec.y;
							TrackedAbilityParticles[msgIndex] = DrawTrajectory(info.begin, info.end);
						}
						break;
					case AP_MEEPO_EARTHBIND:
						if (cpIdx == 0)
							info.begin = cpVal;
						else if (cpIdx == 1) {
							info.end = cpVal;
							TrackedAbilityParticles[msgIndex] = DrawTrajectory(info.begin, info.end);
							Modules::ParticleGC.SetDieTime(TrackedAbilityParticles[msgIndex], 3);
						}
					}
					break;
				}
				case GAME_PARTICLE_MANAGER_EVENT_DESTROY: {
					queuedParticleIndexes.erase(msgIndex);
					if (TrackedAbilityParticles.count(msgIndex)) {
						Modules::ParticleGC.RemoveFromGC(TrackedAbilityParticles[msgIndex]);
						GameSystems::ParticleManager->DestroyParticle(TrackedAbilityParticles[msgIndex]);
						TrackedAbilityParticles.erase(msgIndex);
					}
					break;
				}
				};
			}
		}
	};
}
namespace Modules {
	Hacks::ParticleAbilityWarner ParticleAbilityWarner{};
}