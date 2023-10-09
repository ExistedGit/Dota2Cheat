#pragma once
#include "../Base/VClass.h"
#include "../Base/Vector.h"
#include "../Base/Definitions.h"
#include "CSource2UITexture.h"

#include "CUIPanel.h"

// CPanel2D and its descendants

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
	GETTER(int, GetHeroID, 0xDC); // xref: "heroimagestyle", lesser offset
};

// discovered by me and rebuilt by Morphling
class CDOTA_Hud_ErrorMsg : public CPanel2D {
	PAD(0x28);
public:
	float m_flTotalTime; // 0x30
	float m_flErrorDurationTime; // 0x34

	void SetDialogVariable(const char* variable, const char* value) {
		GetUIPanel()->CallVFunc<292, void>(variable, value);
	}

	void ShowErrorMessage(const char* text) {
		auto panel = GetUIPanel();

		panel->RemoveClass("PopOutEffect");
		panel->ApplyStyles();
		panel->AddClass("ShowErrorMsg");

		panel->RemoveClass("Hidden");
		SetDialogVariable("error_msg", text);
		panel->AddClass("PopOutEffect");
		float flTime = Memory::GetExport("tier0.dll", "Plat_FloatTime").Call<float>();
		m_flTotalTime = flTime + m_flErrorDurationTime;
	}
};