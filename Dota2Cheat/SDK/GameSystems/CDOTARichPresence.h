#pragma once
#include "../Base/VClass.h"

class CDOTARichPresence :public VClass {
public:
	static inline void(*SetRPStatusFunc)(CDOTARichPresence* thisptr, const char* status, int unk, void* unk1);
	void SetRPStatus(const char* status) {
		SetRPStatusFunc(this, status, 0, nullptr);
	}
};