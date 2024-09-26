#pragma once
#include "../Base/Vector.h"
#include "../Base/VClass.h"
#include "../Base/Definitions.h"

class CDOTAPanoramaMinimapRenderer : public VClass {
public:
	PAD(8); //0x0000
	Vector2D MinimapBounds; //0x0010
	PAD(0x9C); //0x0018
	int32_t MinimapSizeX; //0x00AC
	int32_t MinimapSizeY; //0x00B0

	Vector2D GetMinimapSize() {
		return Vector2D((float)MinimapSizeX, (float)MinimapSizeY);
	}
};

