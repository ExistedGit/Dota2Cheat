#pragma once
#include "../../SDK/pch.h"

namespace Modules {
	class TPTracker {
		std::map<CUserMsg_ParticleManager*, CUserMsg_ParticleManager*> MessagePairs;
		std::map<Vector, Vector> TpPoints;
	public:
		enum TPParticle : uint64_t {
			START = 16169843851719108633,
			END = 9908905996079864839
		};
		void ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
			if (msgHandle->messageID != 145)
				return;

			auto pmMsg = reinterpret_cast<CUserMsg_ParticleManager*>(msg);
			auto msgIndex = pmMsg->index();
			switch (pmMsg->type()) {
			case GAME_PARTICLE_MANAGER_EVENT_CREATE: {

				auto particle = pmMsg->create_particle();
				switch (particle.particle_name_index()) {
				case TPParticle::START:
					MessagePairs[pmMsg] = nullptr;
					break;
				case TPParticle::END:
					for (auto& [_, end] : MessagePairs)
						if (!end) end = pmMsg;
					break;
				}

				break;
			}
			case GAME_PARTICLE_MANAGER_EVENT_UPDATE_ENT:
			{
				break;
			}
			// Here we register the actual trajectory
			case GAME_PARTICLE_MANAGER_EVENT_UPDATE_TRANSFORM:
			{
				break;
			}
			case GAME_PARTICLE_MANAGER_EVENT_DESTROY: {
				break;
			}
			};

		};
	};
}