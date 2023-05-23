#include "NetChannel.h"


void Hooks::hkPostReceivedNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits) {
	Hooks::NetChan = thisptr;

	if (messageHandle->messageID != 4 // not CNetMsg_Tick [4]
		&& ctx.gameStage == GameStage::IN_GAME)
	{
		Modules::ShakerAttackAnimFix.ChangeAttackAnimIfNeeded(messageHandle, msg);
		Modules::LinearProjectileWarner.ProcessLinearProjectileMsg(messageHandle, msg);
		Modules::TPTracker.ProcessParticleMsg(messageHandle, msg);
		Modules::ParticleAbilityWarner.ProcessParticleMsg(messageHandle, msg);
		Modules::BlinkRevealer.ProcessParticleMsg(messageHandle, msg);
		Modules::ParticleMaphack.ProcessParticleMsg(messageHandle, msg);
		//Modules::AttackAnimTracker.ProcessAttackAnimMessage(messageHandle, msg);
	}
	return oPostReceivedNetMessage(Hooks::NetChan, messageHandle, msg, type, bits);
}

bool Hooks::hkSendNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type) {
	Hooks::NetChan = thisptr;

	return oSendNetMessage(Hooks::NetChan, messageHandle, msg, type);
}
