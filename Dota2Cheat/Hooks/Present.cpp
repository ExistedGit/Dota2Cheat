#include "Present.h"

#include "../Modules/UI/Indicators/SpeedIndicator.h"
#include "../Modules/UI/Indicators/KillIndicator.h"
// #include "../Modules/UI/Indicators/HookIndicator.h"

#include "../Modules/UI/BlinkRevealer.h"
#include "../Modules/UI/TPTracker.h"
#include "../Modules/UI/ParticleMaphack.h"
#include "../Modules/UI/AbilityESP.h"
#include "../Modules/UI/BarAugmenter.h"

#include "../Modules/Hacks/SkinChanger.h"
#include "../Modules/Hacks/LastHitMarker.h"

LRESULT __stdcall Hooks::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	KeyHandler.OnWindowMessage(uMsg, wParam);

	if (DrawData.ShowMenu) // ImGui_ImplDX11_WndProcHandler
		return ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	else
		return CallWindowProcA(DrawData.Dx.oWndProc, hWnd, uMsg, wParam, lParam);
}

void Hooks::InitImGui() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(DrawData.Dx.Window);
	ImGui_ImplDX11_Init(DrawData.Dx.pDevice, DrawData.Dx.pContext);

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();
}

long Hooks::hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (!DrawData.Initialized) {
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&DrawData.Dx.pDevice))) {
			DrawData.Dx.pDevice->GetImmediateContext(&DrawData.Dx.pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			DrawData.Dx.Window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer = nullptr;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			DrawData.Dx.pDevice->CreateRenderTargetView(pBackBuffer, NULL, &DrawData.Dx.mainRenderTargetView);
			pBackBuffer->Release();
			DrawData.Dx.oWndProc = (WNDPROC)SetWindowLongPtr(DrawData.Dx.Window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();

			Log(LP_INFO, "Loading fonts...");
			auto& io = ImGui::GetIO();
			{
				// ImGui takes ownership of the loaded memory by default
				// Of course, we don't want to try to delete a constant array and get a SEGFAULT
				ImFontConfig fontCfg{};
				fontCfg.FontDataOwnedByAtlas = false;
				for (int i = 2; i < 30; i += 2) {
					DrawData.Fonts["MSTrebuchet"][i] = io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\trebuc.ttf)", i, nullptr, io.Fonts->GetGlyphRangesDefault());
					DrawData.Fonts["Monofonto"][i] = io.Fonts->AddFontFromMemoryTTF((void*)Fonts::Monofonto, IM_ARRAYSIZE(Fonts::Monofonto), i, &fontCfg, io.Fonts->GetGlyphRangesDefault());
				}
			}

			DrawData.ShowMenu = true;
			DrawData.Initialized = true;
		}
		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}
	auto& io = ImGui::GetIO();
	static auto defaultFont = io.Fonts->AddFontDefault();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	texManager.ExecuteLoadCycle();

#ifdef _DEBUG
	// Pages::AutoPickHeroGrid::Draw();
#endif // _DEBUG
	ImGui::PushFont(DrawData.Fonts["MSTrebuchet"][24]);
	if (
		Interfaces::GameUI->GetUIState() == DOTA_GAME_UI_DOTA_INGAME
		&& ctx.gameStage == GameStage::IN_GAME
		&& ctx.localHero
		) {
		Modules::AbilityESP.DrawESP();
		// Modules::UIOverhaul.DrawBars();
		Modules::TPTracker.DrawMapTeleports();
		Modules::LastHitMarker.Draw();
		Modules::BlinkRevealer.Draw();
		Modules::ParticleMaphack.Draw();
		Modules::BarAugmenter.Draw();

		Modules::SpeedIndicator.Draw();
		Modules::KillIndicator.Draw();
	}
	ImGui::PopFont();

	ImGui::PushFont(defaultFont);

#if defined(_DEBUG) && !defined(_TESTING)
	ImGui::InputInt("ItemDef ID", &itemDefId);
	if (ImGui::Button("Create item"))
		Modules::SkinChanger.QueueAddItem(itemDefId);
#endif // _DEBUG


	if (DrawData.ShowMenu)
		Pages::MainMenu::Draw();
	ImGui::PopFont();

	ImGui::EndFrame();
	ImGui::Render();
	DrawData.Dx.pContext->OMSetRenderTargets(1, &DrawData.Dx.mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresent(pSwapChain, SyncInterval, Flags);
}

bool Hooks::GetD3D11SwapchainDeviceContext(void** pSwapchainTable, size_t Size_Swapchain, void** pDeviceTable, size_t Size_Device, void** pContextTable, size_t Size_Context) {
	WNDCLASSEX wc{ 0 };
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = DefWindowProc;
	wc.lpszClassName = TEXT("dummy class");

	if (!RegisterClassEx(&wc))
		return false;

	HWND hWnd = CreateWindow(wc.lpszClassName, TEXT(""), WS_DISABLED, 0, 0, 0, 0, NULL, NULL, NULL, nullptr);

	DXGI_SWAP_CHAIN_DESC swapChainDesc{ 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevel[] =
	{
		D3D_FEATURE_LEVEL_9_1,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0
	};

	IDXGISwapChain* pDummySwapChain = nullptr;
	ID3D11Device* pDummyDevice = nullptr;
	ID3D11DeviceContext* pDummyContext = nullptr;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pDummySwapChain, &pDummyDevice, nullptr, &pDummyContext))) {
		DestroyWindow(swapChainDesc.OutputWindow);
		UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));

		return false;
	}

	if (pSwapchainTable && pDummySwapChain)
		memcpy(pSwapchainTable, *reinterpret_cast<void***>(pDummySwapChain), Size_Swapchain);

	if (pDeviceTable && pDummyDevice)
		memcpy(pDeviceTable, *reinterpret_cast<void***>(pDummyDevice), Size_Device);

	if (pContextTable && pDummyContext)
		memcpy(pContextTable, *reinterpret_cast<void***>(pDummyContext), Size_Context);

	SAFE_RELEASE(pDummySwapChain);
	SAFE_RELEASE(pDummyDevice);
	SAFE_RELEASE(pDummyContext);

	DestroyWindow(swapChainDesc.OutputWindow);
	UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));

	return true;
}
