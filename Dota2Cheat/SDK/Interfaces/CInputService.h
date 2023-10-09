#pragma once
#include "../Base/VClass.h"

class CInputService : public VClass {
public:
	void CmdCommand(const char* command) {
		CallVFunc<25>(5, command, 0);
	}
};