#pragma once
#include "MListeners.h"
#include "../CheatSDK/include.h"

class D2CModule : public IFrameListener, public IEntityListListener {
	virtual bool ListensToNetChan() const { return false; }
	virtual bool ListensToFrames() const { return false; }

	virtual void OnFrame() {};
	
	virtual void Draw() {};
};