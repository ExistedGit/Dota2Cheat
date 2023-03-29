#include "ParticleAbilityWarner.h"
#include "../../SDK/Protobufs/usermessages.pb.h"


// Draws a dashed red line from begin to end
// Returns the wrapper for the created particle

ParticleWrapper Hacks::ParticleAbilityWarner::DrawTrajectory(Vector begin, Vector end) {
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

ParticleWrapper Hacks::ParticleAbilityWarner::DrawRadius(Vector pos, float radius) {
	auto pw = GameSystems::ParticleManager->CreateParticle(
		"particles/units/heroes/hero_snapfire/hero_snapfire_range_finder_aoe.vpcf",
		PATTACH_WORLDORIGIN,
		ctx.assignedHero
	);
	Vector radiusVec{ radius, 0, 0 };
	pw.particle
		->SetControlPoint(0, &pos)
		->SetControlPoint(1, &radiusVec);
	return pw;
}

CDOTABaseNPC_Hero* Hacks::ParticleAbilityWarner::FindParticleOwner(const char* name) {
	for (auto& hero : ctx.heroes) {
		if (auto unitName = hero->GetUnitName())
			if (!strcmp(unitName, name)) {
				return hero;
			}
	}
	return nullptr;
}

void Hacks::ParticleAbilityWarner::ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
	if (msgHandle->messageID != 145)
		return;

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
	case GAME_PARTICLE_MANAGER_EVENT_UPDATE_TRANSFORM:
	{
		if (!queuedParticleIndexes.count(msgIndex))
			break;


		auto& info = queuedParticleIndexes[msgIndex];
		auto cpIdx = pmMsg->update_particle_transform().control_point();
		auto cpVal = Vector{
			pmMsg->update_particle_transform().position().x(),
			pmMsg->update_particle_transform().position().y(),
			pmMsg->update_particle_transform().position().z()
		};

		switch (info.nameIndex) {
		case AP_PUDGE_MEAT_HOOK:
			// Hook's destination
			if (cpIdx == 1) {
				info.end = cpVal;
				auto& data = TrackedAbilityParticles[msgIndex];
				data.trajectory = DrawTrajectory(info.begin, info.end);
				Modules::ParticleGC.SetDieTime(data.trajectory, 3);
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
					]->GetEffectiveCastRange();

						if (info.nameIndex == AP_BANSHEE_CRYPT_SWARM)
							castRange += 390; // hits past its cast range, for some reason

						auto ratio = castRange / cpVal.Length2D();
						auto enlargedVec = cpVal.To<Vector2D>() * ratio;
						info.end = info.begin;
						info.end.x += enlargedVec.x;
						info.end.y += enlargedVec.y;
						TrackedAbilityParticles[msgIndex].trajectory = DrawTrajectory(info.begin, info.end);
			}
			break;
		case AP_MEEPO_EARTHBIND:
			if (cpIdx == 0)
				info.begin = cpVal;
			else if (cpIdx == 1) {
				info.end = cpVal;
				auto& data = TrackedAbilityParticles[msgIndex];
				data.trajectory = DrawTrajectory(info.begin, info.end);
				data.aoe = DrawRadius(info.end, info.owner->GetAbilities()[0]->GetAOERadius());
				Modules::ParticleGC.SetDieTime(data.trajectory, 3);
				Modules::ParticleGC.SetDieTime(data.aoe, 3);
			}
		}
		break;
	}
	case GAME_PARTICLE_MANAGER_EVENT_DESTROY: {
		queuedParticleIndexes.erase(msgIndex);
		if (TrackedAbilityParticles.count(msgIndex)) {
			auto& data = TrackedAbilityParticles[msgIndex];
			Modules::ParticleGC.RemoveFromGC(data.trajectory);
			Modules::ParticleGC.RemoveFromGC(data.aoe);

			GameSystems::ParticleManager->DestroyParticle(data.trajectory);
			GameSystems::ParticleManager->DestroyParticle(data.aoe);

			TrackedAbilityParticles.erase(msgIndex);
		}
		break;
	}
	};

}
