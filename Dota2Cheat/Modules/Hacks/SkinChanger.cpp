#include "SkinChanger.h"

bool Hacks::SkinChanger::AddItem(uint32_t unDefIndex) {
	auto inv = Interfaces::GCClient->GetSOListeners()[1];
	auto soid = inv->GetSOCache()->GetOwner();
	const uint32_t accId = inv->GetSOCache()->GetOwner().m_unSteamID;
	auto item = Signatures::CreateEconItem();

	item->m_unDefIndex = unDefIndex;
	item->m_unAccountID = accId;
	item->m_ulID = itemIdCounter++;
	item->m_unInventory = invPosCounter++;
	item->Flag() = 0x200;

	bool result = inv->GetSOCache()->AddObject(item);
	inv->SOCreated(&soid, item, eSOCacheEvent_Incremental);
	FakeItems[item->m_ulID] = item;
	return result;
}