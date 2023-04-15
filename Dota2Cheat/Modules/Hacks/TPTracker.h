#pragma once
#include "../../SDK/pch.h"
#include "../../Utils/Drawing.h"

namespace Hacks {
	class TPTracker {
		struct ParticleData {
			Vector pos;
			std::string_view name;
			uint32_t msgIdx;
		};


		std::map<CBaseEntity*, ParticleData> tpStarts;
		std::map<CBaseEntity*, ParticleData> tpEnds;

	public:
		void DrawMapTeleports();
		void ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg);;
	};
}
namespace Modules {
	inline Hacks::TPTracker TPTracker{};
}