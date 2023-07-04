#pragma once
#include <Windows.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

#include "../CheatSDK/Data/DrawData.h"
#include "../CheatSDK/KeyHandler.h"
#include "../UI/Pages/MainMenu.h"

// DirectX11's SwapChain::Present, used to draw things

extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hooks {
	typedef long(*PresentFn)(IDXGISwapChain*, UINT, UINT);
	inline PresentFn oPresent;

	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	long hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

	inline void SAFE_RELEASE(auto* t) {
		if (t)
			t->Release();
	}

	bool GetD3D11SwapchainDeviceContext(void** pSwapchainTable, size_t Size_Swapchain, void** pDeviceTable, size_t Size_Device, void** pContextTable, size_t Size_Context);

	inline bool HookDirectX() {
		if (GetD3D11SwapchainDeviceContext(
			DrawData.Dx.SwapChain, sizeof(DrawData.Dx.SwapChain),
			DrawData.Dx.Device, sizeof(DrawData.Dx.Device),
			DrawData.Dx.Context, sizeof(DrawData.Dx.Context)))
			if (HookFunc(DrawData.Dx.SwapChain[8], hkPresent, &oPresent, "Present"))
				return true;

		return false;
	}
}