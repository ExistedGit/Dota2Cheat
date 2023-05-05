#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"
#include "../../Utils/Drawing.h"
#include "MultiThreadModule.h"

#include "consthash/cityhash32.hxx"
#include "cityhash/city.h"

namespace Hacks {
	// Shows the exact point of QoP's and Antimage's blinks
	class BlinkRevealer : public MultiThreadModule {
		struct BlinkData {
			Vector pos{ 0, 0,0 };
			bool qop;
			float fadeCounter, fadeTime;
		};
		std::map<CBaseEntity*, BlinkData> Blinks;

		float lastTime = 0;
	public:
		void Draw();
		void FrameBasedLogic();
		void ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg);
	};
}
namespace Modules {
	inline Hacks::BlinkRevealer BlinkRevealer{};
}