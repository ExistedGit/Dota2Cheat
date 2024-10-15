#include "Network.h"

#include "../Modules/Hacks/ShakerAttackAnimFix.h"
#include "../Modules/Hacks/ParticleAbilityWarner.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/SkinChanger.h"

#include "../Modules/UI/TPTracker.h"
#include "../Modules/UI/BlinkRevealer.h"
#include "../Modules/UI/ParticleMaphack.h"

#include "../Modules/Utility/AttackAnimTracker.h"
#include "../Modules/Utility/OrderRouter.h"
#include <gameevents.pb.h>

bool Hooks::D2CNetFilter::Filter(VClass* netMsg, INetChannel* netchan) {
	auto serializer = netMsg->GetVFunc(3).Call<CNetworkSerializerPB*>();

	if (serializer->messageID == 0    // NetMsgPacketStart
		|| serializer->messageID == 1 // NetMsgPacketEnd
		|| serializer->messageID == net_Tick
		|| serializer->messageID == clc_Move
		|| serializer->messageID == svc_PacketEntities)
		return false;

	auto msg = netMsg->MemberInline<google::protobuf::Message>(0x28);
#ifdef _DEBUG
	if (serializer->messageID == 208) {
		auto seMsg = (CMsgSosStartSoundEvent*)msg;

		static Function FindReplacementSound = Memory::Scan("E8 ? ? ? ? 44 39 BE", "client.dll").GetAbsoluteAddress(1);
		
		if (ctx.localHero) {
			auto idk = CSoundOpSys::Get()->MemberInline<VClass>(8);
			auto s = FindReplacementSound.Call<const char*>(ctx.localHero, idk->GetVFunc(2)(seMsg->soundevent_hash()));

			seMsg->set_soundevent_hash(idk->GetVFunc(0).Call<uint32_t>(s));
		}
	}
#endif

	using namespace Modules;
	static INetChanListener* Listeners[]{
		&ShakerAttackAnimFix,
		&LinearProjectileWarner,
		&TPTracker,
		&ParticleAbilityWarner,
		&BlinkRevealer,
		&ParticleMaphack,
	};

	if (ctx.gameStage == GameStage::IN_GAME)
	{
		for (auto l : Listeners)
			l->OnReceivedMsg(serializer, msg);

		//AttackAnimTracker.ProcessAttackAnimMessage(serializer, (google::protobuf::Message*)((uintptr_t)msg + 0x28));
	}
	if (Config::Debug::LogFilterMessages) {
		Color c{ 255,80,255 };

		Signatures::CMsgColor(&c, "Message: %s\n", serializer->unscopedName);
	}

	return false;
}
