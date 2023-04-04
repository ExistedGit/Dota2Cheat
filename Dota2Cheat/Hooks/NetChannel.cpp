#include "NetChannel.h"
#include "../Modules/Utility/AttackAnimTracker.h"
#include "../Modules/Hacks/SkinChanger.h"


bool Hooks::hkBAsyncSendProto(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID) {
	if (protobufMsg->msgID == k_EMsgClientToGCSetItemStyle) {
		auto msg = (CMsgClientToGCSetItemStyle*)protobufMsg->msg;
		auto item = Modules::SkinChanger.FakeItems[msg->item_id()];
		if (item) {
			item->Style() = msg->style_index();
			Modules::SkinChanger.SOUpdated(item);
		}
	}
	else if (protobufMsg->msgID == k_EMsgClientToGCEquipItems) {
		auto msg = (CMsgClientToGCEquipItems*)protobufMsg->msg;
		auto equip = msg->equips().Get(0);
		if (equip.has_new_slot()) {

			auto item = Modules::SkinChanger.FakeItems[equip.item_id()];
			if (auto& equippedItem = Modules::SkinChanger.EquippedItems[equip.new_class()][equip.new_slot()]) {
				if (equippedItem != item)
					Modules::SkinChanger.Unequip(equippedItem);
			}
			if (item) {
#ifdef _DEBUG
				std::cout << std::format("Equipping {}. Class: {}; Slot: {} | ItemDef: {}\n",
					(void*)item,
					equip.new_slot(),
					equip.new_class(),
					(void*)Signatures::GetItemDefByIndex(Signatures::GetItemSchema(), item->m_unDefIndex)
				);
#endif // _DEBUG
				Modules::SkinChanger.Equip(item, equip.new_class(), equip.new_slot());
				return false;
			}
		};


	}
	return oBAsyncSendProto(protobufMsg, handler, responseMsg, respMsgID);
}

bool Hooks::hkDispatchPacket(CGCClient* thisptr, IMsgNetPacket* netPacket) {
	if (netPacket->GetEMsg() == 26)
#ifdef _DEBUG
		std::cout << "GCClient Recv: " << EDOTAGCMsg2String(netPacket->GetEMsg()) << '\n';
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
