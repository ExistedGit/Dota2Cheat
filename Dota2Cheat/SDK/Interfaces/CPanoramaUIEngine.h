#pragma once
#include "../Base/VClass.h"
#include "../Base/Definitions.h"

class CUIEngineSource2;

// client.dll interface
class CPanoramaUIEngine : public VClass {
public:
	GETTER(CUIEngineSource2*, GetSource2Engine, 0x28);

	static CPanoramaUIEngine* Get() {
		static CPanoramaUIEngine* inst = Memory::GetInterfaceBySubstr("panorama.dll", "PanoramaUIEngine");
		return inst;
	}
};