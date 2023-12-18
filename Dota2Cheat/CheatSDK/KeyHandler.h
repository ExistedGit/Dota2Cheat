#pragma once
#include <Windows.h>
#include "Data/DrawData.h"
#include "Data/UIData.h"
#include "Config.h"
#include "Systems/CheatManager.h"

inline class CKeyHandler {
public:
	void OnKeyUp(WPARAM wParam) {
		switch (wParam) {
		case VK_INSERT: {
			DrawData.ShowMenu = !DrawData.ShowMenu;

			if (UIData::uiState == CheatUIState::LaunchMenu)
				UIData::uiState = CheatUIState::Active;

			if (!DrawData.ShowMenu)
				d2c.SaveConfig();
			break;
		}
		}
	}
	void OnWindowMessage(UINT uMsg, WPARAM wParam) {
		switch (uMsg) {
		case WM_KEYUP: KeyHandler.OnKeyUp(wParam); break;
		}
	}
} KeyHandler;
