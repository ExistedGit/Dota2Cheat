#include "DotaPlusUnlocker.h"
#include "../../CheatSDK/Config.h"
#include "../../pch.h"

// rebuilt from xref: "Failed to find CDOTAGameAccountPlus"

void Modules::M_DotaPlusManager::UpdateDotaPlusStatus() {
	if (!updateSubscription)
		return;

	static auto inventory = Interfaces::GCClient->GetSOListeners()[1];
	static auto objCache = inventory->GetSOCache();

	for (auto& typeCache : objCache->GetTypeCacheList()) {
		if (typeCache->GetEconTypeID() != EEconTypeID::k_CDOTAGameAccountPlus)
			continue;

		updateSubscription = false;

		auto proto = (CSODOTAGameAccountPlus*)typeCache->GetProtobufSO()->GetPObject();

		if (proto->plus_status() == Config::Changer::UnlockDotaPlus)
			return;

		proto->set_plus_flags(!Config::Changer::UnlockDotaPlus);
		proto->set_plus_status(Config::Changer::UnlockDotaPlus);
		Interfaces::GCClient->DispatchSOUpdated(objCache->GetOwner(), typeCache->GetProtobufSO(), eSOCacheEvent_Incremental);

		//for(int i = 1; i <= 124; i++)
		//	SetHeroXP(i, 72050);
	}
}

void Modules::M_DotaPlusManager::SetHeroXP(int heroId, int xp) {
	// I don't know why they do this
	heroId *= 100;

	static auto addr = Memory::Scan("E8 ? ? ? ? 48 8B 5F 78", "client.dll").GetAbsoluteAddress(1);

	EEvent ev = EVENT_ID_PLUS_SUBSCRIPTION;

	// CAsyncDataCache::RequestCurrentData
	static auto dataCache = addr.Offset(0x1a).GetAbsoluteAddress<VClass*>(3);
	static auto RequestCurrentData = addr.Offset(0x1a + 7).GetAbsoluteAddress<Function>(1);

	static auto data = RequestCurrentData.Call<uintptr_t>(dataCache, &ev);

	auto i = *(DWORD*)(data + 0x38);

	struct DataElem {
		uint32_t indexBigger = -1, indexSmaller = -1;
		uint32_t idk, one = 1;
		uint32_t heroId;
		uint32_t xp;
	};

	static_assert(sizeof(DataElem) == 0x18);

	auto arr = *(DataElem**)(data + 0x28);
	DataElem* heroData = nullptr;

	// Then we use binary search to find our data
	do
	{
		auto elem = arr[i];
		auto id = elem.heroId;
		if (heroId >= id)
		{
			// Found our data
			if (heroId == id) {
				heroData = &arr[i];
				break;
			}

			i = elem.indexSmaller;
		}
		else
			i = elem.indexBigger;

	} while (i != -1);

	if (!heroData)
		return;

	heroData->xp = xp;
}
