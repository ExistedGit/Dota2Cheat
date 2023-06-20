#pragma once
#include "../Base/NormalClass.h"

struct CGlobalVars : public NormalClass {
	static CGlobalVars* GetInstance();
};