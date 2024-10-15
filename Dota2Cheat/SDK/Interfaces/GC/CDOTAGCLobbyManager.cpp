#include "CDOTAGCLobbyManager.h"

// Can't be bothered to check what's at these offsets and rename variables
// Just made Morphling's IDA paste a bit more appealing

CDOTALobby* CDOTAGCClientLobbyManager::FindLobby() const {
	auto v1 = Member<uint32_t>(24);
	if (v1 <= 0)
		return 0;

	int v2 = 0;
	uintptr_t v5 = Member<uintptr_t>(16);
	uint64_t i = 0;
	for (; ; i += 64)
	{
		if (v2 >= 0 && v2 < v1 && v2 <= Member<uint32_t>(44)
			&& *(DWORD*)(i + v5) != v2
			&& *(uintptr_t*)(i + v5 + 24))
			break;

		if (++v2 >= v1)
			return nullptr;
	}
	return *(CDOTALobby**)(i + v5 + 24);
}
