#include "NetChannel.h"

bool Hooks::hkBAsyncSendProto(CProtobufMsgBase* protobufMsg, IProtoBufSendHandler* handler, google::protobuf::Message* responseMsg, unsigned int respMsgID) {
	if (protobufMsg->msgID == k_EMsgClientToGCSetItemStyle) {

		auto msg = (CMsgClientToGCSetItemStyle*)protobufMsg->msg;

		if (Modules::SkinChanger.FakeItems.contains(msg->item_id())) {
			auto item = Modules::SkinChanger.FakeItems.at(msg->item_id());
			if (item) {
				item->Style() = msg->style_index();
				Modules::SkinChanger.SOUpdated(item);
			}
		}
	}
	else if (protobufMsg->msgID == k_EMsgClientToGCEquipItems) {
		auto msg = (CMsgClientToGCEquipItems*)protobufMsg->msg;
		auto equip = msg->equips().Get(0);
		if (equip.has_new_slot()) {

			CEconItem* item = nullptr;

			if (Modules::SkinChanger.FakeItems.contains(equip.item_id()))
				item = Modules::SkinChanger.FakeItems.at(equip.item_id());

			CEconItem* equippedItem = nullptr;
			if (Modules::SkinChanger.EquippedItems.contains(equip.new_class())
				&& Modules::SkinChanger.EquippedItems.at(equip.new_class()).contains(equip.new_slot()))
				equippedItem = Modules::SkinChanger.EquippedItems.at(equip.new_class()).at(equip.new_slot());

			if (equippedItem && equippedItem != item)
				Modules::SkinChanger.Unequip(equippedItem);

			if (item) {
#ifdef _DEBUG
				auto itemSchema = Signatures::GetItemSchema();

				auto itemDef = CDOTAItemSchema::GetItemDefByIndex(itemSchema, item->m_unDefIndex);
				LogF(LP_INFO, "Equipping {}. Class: {}; Slot: {} | ItemDef: {}",
					(void*)item,
					equip.new_class(),
					equip.new_slot(),
					(void*)itemDef
				);
#endif // _DEBUG
				for (auto& defaultItem : Modules::SkinChanger.DefaultItems) {
					auto defaultItemDef = itemSchema->GetItemDefByIndexRef(defaultItem.unDefIndex);
					if (defaultItem.unClass == equip.new_class() &&
						defaultItem.szSlot == itemDef->GetSlot()) {
						*defaultItemDef = itemDef;
					}
				}

//				Modules::SkinChanger.Equip(item, equip.new_class(), equip.new_slot());
				return false;
			}
			else {
				LogF(LP_INFO, "Equipping {}. Class: {}; Slot: {}",
					(void*)item,
					equip.new_class(),
					equip.new_slot()
				);
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
		Modules::TPTracker.ProcessParticleMsg(messageHandle, msg);
		Modules::ParticleAbilityWarner.ProcessParticleMsg(messageHandle, msg);
		Modules::AttackAnimTracker.ProcessAttackAnimMessage(messageHandle, msg);
	}

	return oPostReceivedNetMessage(thisptr, messageHandle, msg, type, bits);
}

bool Hooks::hkSendNetMessage(INetChannel* thisptr, NetMessageHandle_t* messageHandle, google::protobuf::Message* msg, NetChannelBufType_t type) {
	return oSendNetMessage(thisptr, messageHandle, msg, type);
}
