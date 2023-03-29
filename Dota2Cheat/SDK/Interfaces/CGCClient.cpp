#include "CGCClient.h"

void CGCClient::DispatchSOUpdated(SOID_t* soid, void* sharedObj, ESOCacheEvent ev) {
	auto listeners = GetSOListeners();
	for (auto& listener : listeners)
		listener->DispatchUpdate(soid, sharedObj, ev);
}


