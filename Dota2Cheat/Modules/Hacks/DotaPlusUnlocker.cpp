#include "DotaPlusUnlocker.h"

// rebuilt from xref: "Failed to find CDOTAGameAccountPlus"

void Modules::DotaPlusUnlocker::UpdateDotaPlusStatus() {
	auto inventory = Interfaces::GCClient->GetSOListeners()[1];

	auto objCache = inventory->GetSOCache();
	for (auto& typeCache : objCache->GetTypeCacheList()) {
		if (typeCache->GetEconTypeID() != EEconTypeID::k_CDOTAGameAccountPlus)
			continue;

		auto message = (CSODOTAGameAccountPlus*)typeCache->GetProtobufSO()->GetPObject();

		if (message->plus_status() == Config::Changer::UnlockDotaPlus)
			return;

		message->set_plus_flags(!Config::Changer::UnlockDotaPlus);
		message->set_plus_status(Config::Changer::UnlockDotaPlus);
		Interfaces::GCClient->DispatchSOUpdated(objCache->GetOwner(), typeCache->GetProtobufSO(), eSOCacheEvent_Incremental);
	}
}
