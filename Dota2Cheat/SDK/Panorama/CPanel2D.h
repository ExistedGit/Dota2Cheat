#pragma once
#include "../Base/VClass.h"
#include "../Base/Vector.h"
#include "../Base/Definitions.h"
#include "CSource2UITexture.h"

class CUIPanel;

class CPanel2D : public VClass {
public:
	struct CImageData : public VClass {
		GETTER(CSource2UITexture*, GetTexture, 0x30);
	};
	struct CImageProxySource : public VClass {
		GETTER(CImageData*, GetImageData, 0x10);
	};
	Vector2D GetPositionWithinWindow() {
		Vector2D result{ 0, 0 };
		CallVFunc<44>(0, &result.x, &result.y);
		return result;
	}
	GETTER(CUIPanel*, GetUIPanel, 0x8);
	GETTER(CImageProxySource*, GetImageProxySource, 0xC8);
};

class CDOTA_UI_HeroImage : public CPanel2D {
public:
	GETTER(const char*, GetSrc, 0x98);
};

