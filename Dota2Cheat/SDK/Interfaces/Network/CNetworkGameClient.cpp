#include "CNetworkGameClient.h"
#include "../../Base/VClass.h"

INetChannel* CNetworkGameClient::GetNetChannel(uint32_t splitscreenSlot) const {
	return ((VClass*)this)->GetVFunc(37).Call<INetChannel*>(splitscreenSlot);
}
