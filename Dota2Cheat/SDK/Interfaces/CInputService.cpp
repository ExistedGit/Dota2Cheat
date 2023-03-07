#include "CInputService.h"

void CInputService::CmdCommand(const char* command) {
	CallVFunc<25>(5, command, 0);
}
