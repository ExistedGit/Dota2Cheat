#pragma once
#include "../Base/Vector.h"
#include "../Base/VClass.h"
#include "../Base/Definitions.h"

class CDOTAPanoramaMinimapRenderer : public VClass {
public:
	PAD(8); //0x0000
	Vector2D MinimapBounds; //0x0010

	Vector2D GetMinimapSize() const {
		return Vector2D(Member<float>(0x10 + 0x9c), Member<float>(0x10 + 0xA0));
	}
};

