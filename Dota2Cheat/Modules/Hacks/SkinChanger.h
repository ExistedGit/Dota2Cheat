#pragma once
#include "../SDK/pch.h"
#include <queue>

// please for the love of god don't interpret this as a complete module
// we still have a long way to go!
namespace Hacks {
	class SkinChanger {
	public:
		std::map<uint32_t, CEconItem*> FakeItems;
		uint32_t itemIdCounter = 0xFFFF,
			invPosCounter = 0xFFFF;

		std::vector<uint32_t> itemsToCreate;
		bool ItemsCreated = false;

		void QueueAddItem(uint32_t unDefIndex) {
			itemsToCreate.push_back(unDefIndex);
			ItemsCreated = true;
		}
		void SOCreated(CEconItem* item) {
			auto inv = Interfaces::GCClient->GetSOListeners()[1];
			auto soid = inv->GetSOCache()->GetOwner();
			inv->SOCreated(&soid, item, eSOCacheEvent_Incremental);
		}
		void SOUpdated(CEconItem* item) {
			auto inv = Interfaces::GCClient->GetSOListeners()[1];
			auto soid = inv->GetSOCache()->GetOwner();
			inv->SOUpdated(&soid, item, eSOCacheEvent_Incremental);
		}
		bool AddItem(uint32_t unDefIndex);
	};
}
namespace Modules {
	inline Hacks::SkinChanger SkinChanger;
}