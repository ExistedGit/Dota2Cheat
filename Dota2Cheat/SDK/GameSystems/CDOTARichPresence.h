#pragma once
#include "../Base/VClass.h"
#include <string_view>

class CDOTARichPresence : public VClass {
public:
	static inline void(*SetRPStatusFunc)(CDOTARichPresence* thisptr, const char* status, int unk, void* unk1) = nullptr;

	void SetRPStatus(std::string_view status) {
		SetRPStatusFunc(this, status.data(), 0, nullptr);
	}
};