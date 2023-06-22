#pragma once
#include "../Base/VClass.h"
#include "../Base/Definitions.h"

//soundsystem.dll
class CSoundOpSystem : public VClass {
public:
	void* StartSoundEvent(void** unk, const char* soundEventName, uint32_t unk2, bool someFlag) {
		return CallVFunc<13>(unk, soundEventName, unk2, someFlag);
	}
};