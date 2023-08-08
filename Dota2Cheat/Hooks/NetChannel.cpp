#include "NetChannel.h"

void Hooks::hkPostReceivedNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits) {
	using namespace Modules;
	static INetChanListener* Listeners[] {
		&ShakerAttackAnimFix,
		&LinearProjectileWarner,
		&TPTracker,
		&ParticleAbilityWarner,
		&BlinkRevealer,
		&ParticleMaphack,
	};

	if (
		messageHandle->messageID != 4 // not CNetMsg_Tick [4]
		&& messageHandle->messageID != 21
		&& ctx.gameStage == GameStage::IN_GAME
		)
	{
		for (auto l : Listeners)
			l->OnReceivedMsg(messageHandle, msg);
		//Modules::AttackAnimTracker.ProcessAttackAnimMessage(messageHandle, msg);
	}
	return ((decltype(&hkPostReceivedNetMessage))oPostReceivedNetMessage)(thisptr, messageHandle, msg, type, bits);
}

bool Hooks::hkSendNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type) {
	//if (messageHandle->messageID == 4
	//	|| ctx.gameStage != GameStage::IN_GAME)
	//	return oSendNetMessage(thisptr, messageHandle, msg, type);

	//Modules::OrderRouter.RouteOrder(messageHandle, msg);
		
	return ((decltype(&hkSendNetMessage))oSendNetMessage)(thisptr, messageHandle, msg, type);
}
