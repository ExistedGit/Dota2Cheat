#include "AutoPing.h"

void Modules::M_AutoPing::OnFrame() {
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
	auto handle = Interfaces::NetworkMessages->FindNetworkMessage("CDOTAClientMsg_MapPing");
	//if (Hooks::NetChan)
	//	Hooks::oSendNetMessage(Hooks::NetChan, handle, msg, BUF_DEFAULT);
}
