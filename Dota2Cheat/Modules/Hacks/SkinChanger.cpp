#include "SkinChanger.h"

void Modules::SkinChanger::ParseItemDefs(std::istream& stream) {
	using namespace nlohmann;
	json data = json::parse(stream);
	for (auto& pair: data["items"].items()) {
		int defIdx = pair.value();
		QueueAddItem(defIdx);
	}
	//for (auto& pair: data["default_items"].items()) {
	//	uint32_t defIdx = pair.value().get<uint32_t>();
	//	auto itemDef = CDOTAItemSchema::GetItemDefByIndex(Signatures::GetItemSchema(), defIdx);
	//	DefaultItems.push_back(ItemData{
	//		.unDefIndex = defIdx,
	//		.unClass = itemDef->GetClass(),
	//		.szSlot = itemDef->GetSlot()
	//		});
	//}

}

// structure reversed from CEconItem::IsStyleUnlocked
// xref: "unlocked styles"
void Modules::SkinChanger::UnlockAllStyles(CEconItem* pItem) {
	using namespace Signatures;
	auto itemSchema = GetItemSchema();
	auto itemDef = CDOTAItemSchema::GetItemDefByIndex(itemSchema, pItem->m_unDefIndex);
	auto styles = itemDef->GetAssetModifierContainer()->GetStyles();

	if (!styles)
		return;

	for (auto style : *styles) {
		style->Field<uintptr_t>(0x50) =
			style->Field<uintptr_t>(0x40) =
			style->Field<uint32_t>(0x30) = 0;
	}
}
void Modules::SkinChanger::Equip(CEconItem* pItem, uint16_t unClass, uint16_t unSlot) {
	EquippedItems[unClass][unSlot] = pItem;

	// pItem->EnsureCustomDataExists();

	pItem->Class() = unClass;
	pItem->Slot() = unSlot;
	pItem->Flag() = 3;

	SOUpdated(pItem);
}
CEconItem* Modules::SkinChanger::CreateItem(uint32_t unDefIndex) {
	static auto inv = Interfaces::GCClient->GetSOListeners()[1];
	static auto soid = inv->GetSOCache()->GetOwner();
	const uint32_t accId = soid.m_unSteamID;
	auto item = Signatures::CreateEconItem();
	CSOEconItem proto;
	proto.set_account_id(accId);
	proto.set_inventory(invPosCounter++);
	proto.set_id(itemIdCounter++);
	proto.set_def_index(unDefIndex);
	proto.set_flags(2);
	proto.set_origin(kEconItemOrigin_Earned);
	item->DeserializeFromProtobufItem(&proto);

	return item;
}
bool Modules::SkinChanger::AddItem(uint32_t unDefIndex) {
	static auto inv = Interfaces::GCClient->GetSOListeners()[1];
	static auto soid = inv->GetSOCache()->GetOwner();
	const uint32_t accId = soid.m_unSteamID;
	auto item = Signatures::CreateEconItem();
	CSOEconItem proto;
	proto.set_account_id(accId);
	proto.set_inventory(invPosCounter++);
	proto.set_id(itemIdCounter++);
	proto.set_def_index(unDefIndex);
	proto.set_flags(2);
	proto.set_origin(kEconItemOrigin_Earned);
	item->DeserializeFromProtobufItem(&proto);

	LogF(LP_INFO, "Created item with ID {}", proto.id());

	// UnlockAllStyles(item);

	bool result = inv->GetSOCache()->AddObject(item);
	inv->SOCreated(&soid, item, eSOCacheEvent_Incremental);
	FakeItems[proto.id()] = item;

	if (Modules::SkinChanger.itemsToEquip.contains(unDefIndex)) {
		auto equip = Modules::SkinChanger.itemsToEquip.at(unDefIndex);
		Modules::SkinChanger.Equip(item, equip.unClass, equip.unSlot);
	}

	return result;
}
