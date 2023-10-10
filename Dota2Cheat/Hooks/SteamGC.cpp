#include "SteamGC.h"
#include "../Modules/Hacks/AutoAccept.h"
#include "../Modules/Hacks/SkinChanger.h"

EGCResults Hooks::hkSendMessage(ISteamGameCoordinator* thisptr, uint32_t unMsgType, const void* pubData, uint32_t cubData) {
	const auto msgId = unMsgType & 0x7FFFFFFF;
	auto data = (void*)((std::uintptr_t)pubData + 8);
	auto size = cubData - 8;

	//if (protobufMsg->msgID == k_EMsgClientToGCSetItemStyle) {

	//	auto msg = (CMsgClientToGCSetItemStyle*)protobufMsg->msg;

	//	if (Modules::SkinChanger.FakeItems.contains(msg->item_id())) {
	//		auto item = Modules::SkinChanger.FakeItems.at(msg->item_id());
	//		if (item) {
	//			item->Style() = msg->style_index();
	//			Modules::SkinChanger.SOUpdated(item);
	//		}
	//	}
	//}
	//else
	if (msgId == 7070) {
		CMsgReadyUp msg;
		if (!msg.ParsePartialFromArray(data, size))
			return oSendMessage(thisptr, unMsgType, pubData, cubData);

		Log(LP_INFO, msg.ready_up_key());
	}
	if (msgId == k_EMsgClientToGCEquipItems) {
		CMsgClientToGCEquipItems msg;
		if(!msg.ParsePartialFromArray(data, size))
			return oSendMessage(thisptr, unMsgType, pubData, cubData);

		auto equip = msg.equips().Get(0);
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
				auto itemSchema = Signatures::GetItemSchema();

				auto itemDef = CDOTAItemSchema::GetItemDefByIndex(itemSchema, item->m_unDefIndex);

				LogF(LP_INFO, "Equipping {}. Class: {}; Slot: {} | ItemDef: {}",
					(void*)item,
					equip.new_class(),
					equip.new_slot(),
					(void*)itemDef
				);

				Modules::SkinChanger.Equip(item, equip.new_class(), equip.new_slot());
				return k_EGCResultOK;
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

	return oSendMessage(thisptr, unMsgType, pubData, cubData);
}

EGCResults Hooks::hkRetrieveMessage(ISteamGameCoordinator* thisptr, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize) {
	const auto ret = (decltype(&hkRetrieveMessage)(oRetrieveMessage))(thisptr, punMsgType, pubDest, cubDest, pcubMsgSize);

	const auto msg_id = *punMsgType & 0x7FFFFFFF;
	auto body_data = (void*)(((std::uintptr_t)pubDest) + 8);
	auto body_size = *pcubMsgSize - 8;
	if (msg_id == k_EMsgGCReadyUpStatus) {
		//CMsgReadyUpStatus body;
		//if (body.ParsePartialFromArray(body_data, body_size)) {
		//	static auto steamID = Interfaces::GCClient->GetSOListeners()[1]->GetSOCache()->GetOwner();
		//	auto accepted = body.accepted_ids();
		//	bool hasOwnId = false;
		//	for (auto& id : accepted) {

		//		if (id == steamID.m_unSteamID) {
		//			hasOwnId = true;
		//			break;
		//		}
		//	}
		//	if (!hasOwnId)
		//}
		Modules::AutoAccept.AcceptMatch();
	}

	return ret;
};
