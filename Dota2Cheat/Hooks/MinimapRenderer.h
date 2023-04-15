#pragma once
#include "../SDK/pch.h"

// Using a hook of MinimapRenderer's Render function to get it

namespace Hooks {
	using namespace Signatures;
	inline CDOTAMinimapRenderer_RenderFn oCDOTAMinimapRenderer_Render{};
	inline void* hkCDOTAMinimapRenderer_Render(CDOTAPanoramaMinimapRenderer* thisptr, void* unk0, void** unk1, void* unk2, float unk3, float unk4, float unk5, float unk6) {
		if (!GameSystems::MinimapRenderer)
			GameSystems::MinimapRenderer = thisptr;
		return oCDOTAMinimapRenderer_Render(thisptr, unk0, unk1, unk2, unk3, unk4, unk5, unk6);
	}
}