#pragma once

#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "../VTableIndexes.h"
#include <span>
#include <string_view>
#include "CUIPanel.h"
#include "CLocalization.h"

class PanelListNode {
	void* unk0, * unk1;
public:
	CUIPanel* uiPanel;
private:
	void* unk2;
};

class CUIEngineSource2 : public VClass {

public:
	template<uint32_t size>
	auto GetPanelList() {
		return std::span{ Member<PanelListNode*>(0xf8), size };
	}

	uint16_t MakeSymbol(const char* string) {
		uint16_t result = 0;
		return CallVFunc<121, uint16_t>(&result, string);
	}

	bool IsValidPanelPointer(CUIPanel* panel) {
		struct  IVPBuffer {
			uint32_t unk0, unk1;
		} buf;
		return CallVFunc<VTableIndexes::CUIEngineSource2::IsValidPanelPointer, IVPBuffer*>(&buf, panel)->unk0 != 0xFFFFFFFF;
	}
	FIELD(CUtlVector<void(*)()>, GetListeners, 0xD8);
	// via xref: "CPanel2D::SetDialogVariable - char *"
	VGETTER(CLocalization*, GetLocalization, 19);
};