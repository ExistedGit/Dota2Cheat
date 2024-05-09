#pragma once
#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "CSource2UITexture.h"

class CUIRenderDeviceSource2 : public VClass {
public:
	// This does not decode YCoCg for BC3-encoded VPKs, useless.
	virtual bool LoadVPK(CSource2UITexture** tex, const char* path) = 0;
};