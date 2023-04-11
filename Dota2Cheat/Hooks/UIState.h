#pragma once
#include "../SDK/pch.h"
#include "../UI/DrawContext.h"
namespace Hooks {
	inline Signatures::CGameUI__ChangeGameUIStateFn oCGameUI__ChangeGameUIState;

	inline void* hkCGameUI__ChangeGameUIState(void* thisptr, DOTAGameUIState_t state) {
		draw_ctx.uiState = state;
		return oCGameUI__ChangeGameUIState(thisptr, state);
	}
}