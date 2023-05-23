#pragma once
#include "../SDK/pch.h"
#include "../SDK/Interfaces/Network/INetChannel.h"

#include "../Modules/Hacks/ShakerAttackAnimFix.h"
#include "../Modules/Hacks/ParticleAbilityWarner.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/SkinChanger.h"

#include "../Modules/UI/TPTracker.h"
#include "../Modules/UI/BlinkRevealer.h"
#include "../Modules/UI/ParticleMaphack.h"

#include "../Modules/Utility/AttackAnimTracker.h"

namespace Hooks {
	inline INetChannel* NetChan{};

	typedef void(__fastcall* PostReceivedNetMessageFn)(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits);
	inline PostReceivedNetMessageFn oPostReceivedNetMessage{};
	void hkPostReceivedNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits);

	typedef bool(__fastcall* SendNetMessageFn)(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type);
	inline SendNetMessageFn oSendNetMessage{};
	bool hkSendNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type);
}