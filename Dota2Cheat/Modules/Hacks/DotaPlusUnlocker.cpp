#include "DotaPlusUnlocker.h"

// rebuilt from xref: "Failed to find CDOTAGameAccountPlus"

void Modules::DotaPlusUnlocker::UpdateDotaPlusStatus() {
	if (!updateQueued)
		return;

	static auto inventory = Interfaces::GCClient->GetSOListeners()[1];
	static auto objCache = inventory->GetSOCache();

	for (auto& typeCache : objCache->GetTypeCacheList()) {
		if (typeCache->GetEconTypeID() != EEconTypeID::k_CDOTAGameAccountPlus)
			continue;

		updateQueued = false;

		auto proto = (CSODOTAGameAccountPlus*)typeCache->GetProtobufSO()->GetPObject();

		if (proto->plus_status() == Config::Changer::UnlockDotaPlus)
			return;

		proto->set_plus_flags(!Config::Changer::UnlockDotaPlus);
		proto->set_plus_status(Config::Changer::UnlockDotaPlus);
		Interfaces::GCClient->DispatchSOUpdated(objCache->GetOwner(), typeCache->GetProtobufSO(), eSOCacheEvent_Incremental);
	}
}
