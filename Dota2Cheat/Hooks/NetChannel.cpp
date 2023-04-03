#include "NetChannel.h"
#include "../Modules/Utility/AttackAnimTracker.h"
#include "../Modules/Hacks/SkinChanger.h"


bool Hooks::hkBAsyncSendProto(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID) {
	if (protobufMsg->msgID == k_EMsgClientToGCEquipItems) {
		auto msg = (CMsgClientToGCEquipItems*)protobufMsg->msg;
		auto equip = msg->equips().Get(0);
		if (Modules::SkinChanger.FakeItems.count(equip.item_id())) {
			auto item = Modules::SkinChanger.FakeItems[equip.item_id()];
			if (equip.has_new_slot()) {
#ifdef _DEBUG
				std::cout << std::format("Equipping {}. Class: {}; Slot: {}\n",
					(void*)item,
					equip.new_slot(),
					equip.new_class()
				);
#endif // _DEBUG
				item->Class() = equip.new_class();
				item->Slot() = equip.new_slot();
				item->Flag() = 0x200 | 0x100;

				Modules::SkinChanger.SOUpdated(item);
				return false;
			}
		};


	}
	return oBAsyncSendProto(protobufMsg, handler, responseMsg, respMsgID);
}

bool Hooks::hkDispatchPacket(CGCClient* thisptr, IMsgNetPacket* netPacket) {
#ifdef _DEBUG
	//	std::cout << "GCClient Recv: " << std::dec << EDOTAGCMsg2String(netPacket->GetEMsg()) << '\n';
#endif // _DEBUG
	return oDispatchPacket(thisptr, netPacket);
}

void Hooks::hkPostReceivedNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, void const* type, int bits) {
	if (messageHandle->messageID != 4) // not CNetMsg_Tick [4]
	{
		NetMessageInfo_t* info = Interfaces::NetworkMessages->GetNetMessageInfo(messageHandle);
		const char* name = info->pProtobufBinding->GetName();

		Modules::ShakerAttackAnimFix.ChangeAttackAnimIfNeeded(messageHandle, msg);
		Modules::LinearProjectileWarner.ProcessLinearProjectileMsg(messageHandle, msg);
		Modules::ParticleAbilityWarner.ProcessParticleMsg(messageHandle, msg);
		Modules::AttackAnimTracker.ProcessAttackAnimMessage(messageHandle, msg);
	}

	return oPostReceivedNetMessage(thisptr, messageHandle, msg, type, bits);
}

bool Hooks::hkSendNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type) {
	return oSendNetMessage(thisptr, messageHandle, msg, type);
}
