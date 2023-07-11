#include "NetChannel.h"

void Hooks::hkPostReceivedNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits) {
	NetChan = thisptr;

	if (
		messageHandle->messageID != 4 // not CNetMsg_Tick [4]
		&& messageHandle->messageID != 21
		&& ctx.gameStage == GameStage::IN_GAME
		)
	{
		Modules::ShakerAttackAnimFix.OnReceivedMsg(messageHandle, msg);
		Modules::LinearProjectileWarner.OnReceivedMsg(messageHandle, msg);
		Modules::TPTracker.OnReceivedMsg(messageHandle, msg);
		Modules::ParticleAbilityWarner.OnReceivedMsg(messageHandle, msg);
		Modules::BlinkRevealer.OnReceivedMsg(messageHandle, msg);
		Modules::ParticleMaphack.OnReceivedMsg(messageHandle, msg);
		//Modules::AttackAnimTracker.ProcessAttackAnimMessage(messageHandle, msg);
	}
	return ((decltype(&hkPostReceivedNetMessage))oPostReceivedNetMessage)(thisptr, messageHandle, msg, type, bits);
}

bool Hooks::hkSendNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type) {
	NetChan = thisptr;

	//if (messageHandle->messageID == 4
	//	|| ctx.gameStage != GameStage::IN_GAME)
	//	return oSendNetMessage(thisptr, messageHandle, msg, type);

	//Modules::OrderRouter.RouteOrder(messageHandle, msg);
		
	return ((decltype(&hkSendNetMessage))oSendNetMessage)(thisptr, messageHandle, msg, type);
}
