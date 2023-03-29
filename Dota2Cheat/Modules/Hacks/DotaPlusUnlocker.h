#pragma once
#include "../../SDK/pch.h"
#include "../../Config.h"

namespace Hacks {
	class DotaPlusUnlocker {
	public:
		// rebuilt from xref: "Failed to find CDOTAGameAccountPlus"
		void UpdateDotaPlusStatus() {
			auto inventory = Interfaces::GCClient->GetSOListeners()[1];

			auto objCache = inventory->GetSOCache();
			for (auto& typeCache : objCache->GetTypeCacheList()) {
				if (typeCache->GetSomeKindOfIndex() < 2012) // CDOTAGameAccountPlus' index
					continue;

				auto message = (CDOTAGameAccountPlus*)typeCache->GetProtobufSO()->GetPObject();
				message->set_plus_flags(!Config::UnlockDotaPlus);
				message->set_plus_status(Config::UnlockDotaPlus);
				Interfaces::GCClient->DispatchSOUpdated(objCache->GetOwner(), typeCache->GetProtobufSO(), eSOCacheEvent_Incremental);
			}
		}
	};
}
namespace Modules {
	inline Hacks::DotaPlusUnlocker DotaPlusUnlocker{};
}