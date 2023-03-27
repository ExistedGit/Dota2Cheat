#pragma once
#include "../../SDK/pch.h"

namespace Modules {
	class TPTracker {
		struct ParticleData {
			Vector pos;
			std::string_view name;
			CBaseEntity* ent;
		};
		std::map<int, ParticleData> tpPoints;
	public:
		void ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
			if (msgHandle->messageID != 145)
				return;

			auto pmMsg = reinterpret_cast<CUserMsg_ParticleManager*>(msg);
			auto msgIndex = pmMsg->index();
			switch (pmMsg->type()) {
			case GAME_PARTICLE_MANAGER_EVENT_CREATE: {

				auto particle = pmMsg->create_particle();
				if (!particle.has_particle_name_index())
					break;
				std::string_view particleName = "";
				{
					const char* name = Interfaces::ResourceSystem->GetResourceName(particle.particle_name_index());
					if (name)
						particleName = name;
				}
				if (particleName != "particles/items2_fx/teleport_start.vpcf" &&
					particleName != "particles/items2_fx/teleport_end.vpcf")
					break;

				tpPoints[msgIndex] = ParticleData{
					.name = particleName,
					.ent = Interfaces::EntitySystem->GetEntity(NH2IDX(pmMsg->create_particle().entity_handle()))
				};
			}
												   // Here we register the actual trajectory
			case GAME_PARTICLE_MANAGER_EVENT_UPDATE_TRANSFORM:
			{
				int cp = pmMsg->update_particle_transform().control_point();
				if (cp != 0)
					break;

				auto pos = pmMsg->update_particle_transform().position();

				if (!tpPoints.count(msgIndex))
					break;
				auto& data = tpPoints[msgIndex];
				data.pos = Vector(pos.x(), pos.y(), pos.z());
				break;
			}
			case GAME_PARTICLE_MANAGER_EVENT_DESTROY: {
				tpPoints.erase(msgIndex);
				break;
			}
			};

		};
	};
}