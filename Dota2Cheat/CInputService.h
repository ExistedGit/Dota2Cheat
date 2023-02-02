#pragma once
#include "sdk.h"
class CInputService : public VClass {
public:
	inline void CmdCommand(const char* command) {
		CallVFunc<25>(5, command, 0);
	}
};