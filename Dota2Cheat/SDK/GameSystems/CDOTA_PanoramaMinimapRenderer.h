#pragma once
#include "../Base/Vector.h"

class CDOTAPanoramaMinimapRenderer {
public:
	char pad_0000[16]; //0x0000
	Vector2D MinimapBounds; //0x0010
	char pad_0018[148]; //0x0018
	int32_t MinimapSizeX; //0x00AC
	int32_t MinimapSizeY; //0x00B0

	Vector2D GetMinimapSize() {
		return Vector2D((float)MinimapSizeX, (float)MinimapSizeY);
	}
};

