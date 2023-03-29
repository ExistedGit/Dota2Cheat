#include "CGCClient.h"

void CGCClient::DispatchSOUpdated(SOID_t* soid, void* sharedObj, ESOCacheEvent ev) {
	auto listeners = GetSOListeners();
	for (auto& listener : listeners)
		listener->DispatchUpdate(soid, sharedObj, ev);
}

// not working
// Liberalist's method must be obsolete
void CGCClient::FindCDOTAGameAccountPlus() {
	auto inventory = GetSOListeners()[1];

	auto objCache = inventory->GetSOCache();
	for (auto& typeCache : objCache->GetTypeCacheList()) {
		if (typeCache->GetSomeKindOfIndex() < 2012) // CDOTAGameAccountPlus' index
			continue;

		auto message = (CDOTAGameAccountPlus*)typeCache->GetProtobufSO()->GetPObject();
		message->set_plus_flags(0);
		message->set_plus_status(1);
		DispatchSOUpdated(objCache->GetOwner(), typeCache->GetProtobufSO(), eSOCacheEvent_Incremental);
	}
}
