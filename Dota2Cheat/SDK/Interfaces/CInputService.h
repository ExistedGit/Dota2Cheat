#pragma once
#include "../Base/VClass.h"

class CInputService : public VClass {
public:
	void CmdCommand(std::string_view command) {
		CallVFunc<25>(5, command.data(), 0);
	}
};