#pragma once
#include <imgui/imgui.h>
#include <string>
#include <map>
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <algorithm>

inline struct CDrawData {
	bool Initialized = false;

	// some people love to play with their Dota HUD flipped
	bool IsHUDFlipped = false;
	std::map<std::string, std::map<int, ImFont*>> Fonts;
	bool ShowMenu = false;

	// Clamps the size to 10-26 range and rounds the font size to the nearest even number
	ImFont* GetFont(const std::string& name, int size) {
		size = std::clamp(size, 10, 26);
		if (size % 2 != 0)
			++size;

		return Fonts[name][size];
	}

	struct CDx11Data {
		HWND Window{};
		void* SwapChain[18];
		void* Device[40];
		void* Context[108];
		WNDPROC oWndProc{};
		ID3D11Device* pDevice = NULL;
		ID3D11DeviceContext* pContext = NULL;
		ID3D11RenderTargetView* mainRenderTargetView = NULL;
	} Dx;

} DrawData;