#pragma once
#include "../SDK/pch.h"

// I mean, I tried
// maybe someone else figures it out.
namespace Hacks {
	class SkinChanger {
	public:
		std::map<uint32_t, CEconItem*> FakeItems;
		std::map<uint16_t, std::map<uint16_t, CEconItem*>> EquippedItems;

		uint32_t itemIdCounter = 0xFFFF,
			invPosCounter = 0xFFFF;

		std::vector<uint32_t> itemsToCreate;
		bool ItemsCreated = false;

		// structure reversed from CEconItem::IsStyleUnlocked
		// xref: "unlocked styles"
		void UnlockAllStyles(CEconItem* pItem) {
			using namespace Signatures;
			auto itemSchema = GetItemSchema();
			auto itemDef = GetItemDefByIndex(itemSchema, pItem->m_unDefIndex);
			auto styles = itemDef->GetAssetModifierContainer()->GetStyles();

			if (!styles)
				return;

			for (auto style : *styles) {
				style->Field<uintptr_t>(0x50) =
					style->Field<uintptr_t>(0x40) =
					style->Field<uint32_t>(0x30) = 0;
			}
		}

		// Call in main thread
		void Equip(CEconItem* pItem, uint16_t unClass, uint16_t unSlot) {
			EquippedItems[unClass][unSlot] = pItem;

			pItem->Class() = unClass;
			pItem->Slot() = unSlot;
			pItem->Flag() = 3;

			SOUpdated(pItem);
		}
		// Call in main thread
		void Unequip(CEconItem* pItem) {
			EquippedItems[pItem->Class()][pItem->Slot()] = nullptr;

			pItem->Class() = 0;
			pItem->Slot() = static_cast<uint16_t>(-1);
			pItem->Flag() = 2;

			SOUpdated(pItem);
		}

		void QueueAddItem(uint32_t unDefIndex) {
			itemsToCreate.push_back(unDefIndex);
			ItemsCreated = true;
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