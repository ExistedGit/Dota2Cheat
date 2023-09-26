#pragma once

#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "../VTableIndexes.h"
#include <span>
#include <string_view>
#include "CUIPanel.h"
#include "CLocalization.h"
#include "CUIRenderDeviceSource2.h"

class PanelListNode {
	void* unk0, *unk1;
public:
	CUIPanel* uiPanel;
private:
	void* unk2;
};

class CUIEngineSource2 : public VClass {

public:
	template<uint32_t size>
	auto GetPanelList() {
		return std::span{ Member<PanelListNode*>(0xf0), size };
	}

	GETTER(CUIRenderDeviceSource2*, GetRenderDevice, 0xBA8);

	uint16_t MakeSymbol(const char* string) {
		uint16_t result = 0;
		CallVFunc<121, uint16_t>(&result, string);
		return result;
	}

	bool IsValidPanelPointer(CUIPanel* panel) {
		struct  IVPBuffer {
			uint32_t unk0, unk1;
		} buf;
		return CallVFunc<VTableIndexes::CUIEngineSource2::IsValidPanelPointer, IVPBuffer*>(&buf, panel)->unk0 != 0xFFFFFFFF;
	}
	// Iterated in RunFrame
	FIELD(CUtlVector<void(*)()>, GetListeners, 0xD0);
	// decompile xref: "CPanel2D::SetDialogVariable - char *"
	// above it is a vfunc call by g_pUIEngine
	VGETTER(CLocalization*, GetLocalization, 19);
};