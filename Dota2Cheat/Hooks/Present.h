#pragma once
#include <Windows.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

#include "../CheatSDK/Data/DrawData.h"
#include "../CheatSDK/KeyHandler.h"
#include "../UI/Pages/MainMenu.h"

// DirectX11's SwapChain::Present, used to render things
// Its hooking is different from the "classical" method of creating a dummy window
// The first problem is that the Steam overlay doesn't work after such manipulations
// The second problem is that that same overlay's own hkPresent is called by our one,
// creating a possibility for Valve to make a retar~ retaddr check and detect the cheat
// Not saying they made one, but we don't take any chances.

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hooks {
	typedef long(*PresentFn)(IDXGISwapChain*, UINT, UINT);
	inline PresentFn oPresent;

	LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	long hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

	inline bool HookDirectX() {
		// xref: "Hooking vtable for swap chain\n"
		auto Present = SignatureDB::FindSignature("IDXGISwapChain::Present").Dereference();
		return HOOKFUNC(Present);
	}
}