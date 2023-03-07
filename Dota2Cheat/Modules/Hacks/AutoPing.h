#pragma once
#include "../../SDK/include.h"
#include "../../Hooks/NetChannel.h"
#include "../../SDK/Protobufs/dota_clientmessages.pb.h"

namespace Config {
	bool AutoPingEnabled = false;
	CDOTABaseNPC_Hero* AutoPingTarget = 0;
	float AutoPingDelay = 1;
}

namespace Hacks {
	class AutoPing {
		float lastGameTime = 0;
	public:
		void FrameBasedLogic() {
			if (!Config::AutoPingEnabled ||
				!Config::AutoPingTarget ||
				lastGameTime == 0) {
				lastGameTime = GameSystems::GameRules->GetGameTime();
				return;
			}

			float delta = GameSystems::GameRules->GetGameTime() - lastGameTime;
			if (delta < Config::AutoPingDelay)
				return;

			lastGameTime = GameSystems::GameRules->GetGameTime();

			CDOTAClientMsg_MapPing msg;
			auto loc = msg.mutable_location_ping();
			loc->set_x(Config::AutoPingTarget->GetPos().x);
			loc->set_y(Config::AutoPingTarget->GetPos().y);
			loc->set_target(-1);
			loc->set_direct_ping(false);
			loc->set_type(0);
			loc->set_ping_source(k_ePingSource_Default);
			auto handle = Interfaces::NetworkMessages->FindNetworkMessage2("CDOTAClientMsg_MapPing");
			//if (Hooks::NetChan)
			//	Hooks::oSendNetMessage(Hooks::NetChan, handle, msg, BUF_DEFAULT);
		}
	};
}
namespace Modules {
	Hacks::AutoPing AutoPing;
}