#pragma once
#include "../Base/VClass.h"

class CDOTAGCClientSystem : public VClass {
public:
	void SaveSerializedSOCache() {
		CallVFunc<2>();
	}

	static CDOTAGCClientSystem* Get();
};
