#include "SkinChanger.h"

void Hacks::SkinChanger::ParseItemDefs(std::istream& stream) {
	using namespace nlohmann;
	json data = json::parse(stream);
	for (auto& pair: data["items"].items()) {
		int defIdx = pair.value().get<int>();
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
void Hacks::SkinChanger::UnlockAllStyles(CEconItem* pItem) {
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

bool Hacks::SkinChanger::AddItem(uint32_t unDefIndex) {
	auto inv = Interfaces::GCClient->GetSOListeners()[1];
	auto soid = inv->GetSOCache()->GetOwner();
	const uint32_t accId = inv->GetSOCache()->GetOwner().m_unSteamID;
	auto item = Signatures::CreateEconItem();

	item->m_unDefIndex = unDefIndex;
	item->m_unAccountID = accId;
	item->m_ulID = itemIdCounter++;
	item->m_unInventory = invPosCounter++;
	item->Flag() = 2;
	UnlockAllStyles(item);

	bool result = inv->GetSOCache()->AddObject(item);
	inv->SOCreated(&soid, item, eSOCacheEvent_Incremental);
	FakeItems[item->m_ulID] = item;
	return result;
}
