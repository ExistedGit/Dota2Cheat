#pragma once
#include <Windows.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

#include "../CheatSDK/Data/DrawData.h"
#include "../CheatSDK/KeyHandler.h"
#include "../UI/Pages/MainMenu.h"

inline std::mutex writeMutex;
struct NetvarWriteData {
	int writeCount = 0;
	int lastVal{};
};
inline std::map<std::string, NetvarWriteData> writes;

// DirectX11's SwapChain::Present, used to render things
// Its hooking is different from the "classical" method of creating a dummy window
// We just hook the overlay's hook!

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hooks {
	typedef long(*PresentFn)(IDXGISwapChain*, UINT, UINT);
	inline PresentFn oPresent;

	LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	long hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

	inline bool HookDirectX() {
		// xref: "Hooking vtable for swap chain\n"
		auto Present = SignatureDB::FindSignature("IDXGISwapChain::Present");
		return HOOKFUNC(Present);
	}

	inline bool HookDX11Old() {
		HWND hWnd = GetForegroundWindow();
		IDXGISwapChain* pSwapChain;

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = TRUE;//((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;

		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1
			, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, NULL, &pContext)))
		{
			MessageBox(hWnd, "Failed to create directX device and swapchain!", "uBoos?", MB_ICONERROR);
			return false;
		}


		auto Present = (*(void***)pSwapChain)[8];
		auto res = HOOKFUNC(Present);


		pDevice->Release();
		pContext->Release();
		pSwapChain->Release();

		return res;
	}
}