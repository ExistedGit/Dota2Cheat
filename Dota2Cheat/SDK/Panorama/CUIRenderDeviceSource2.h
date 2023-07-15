#pragma once
#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "CSource2UITexture.h"

class CUIRenderDeviceSource2 : public VClass {
public:
	bool LoadVPK(CSource2UITexture** tex, const char* path) {
		return CallVFunc<0, bool>(tex, path);
	}
};