#pragma once
#include "../pch.h"

#include "../Hooks/PrepareUnitOrders.h"
#include "../Hooks/AcceptEvents.h"
#include "../Hooks/FrameStageNotify.h"
#include "../Hooks/Network.h"
#include "../Hooks/GameCoordinator.h"
#include "../Hooks/ParticleRendering.h"
#include "../Hooks/SteamGC.h"
#include "../Hooks/ModifierEvents.h"
#include "../Hooks/Present.h"

#include "../Hooks/Misc.h"

namespace Hooks {
	void InstallHooks();

	inline void InstallAuxiliaryHooks() {
		CEntSys::Get()->GetListeners().push_back(&EntityList);

		auto cl = CSchemaSystem::Get()->FindTypeScopeForModule("engine2.dll")->FindDeclaredClass("EventFrameBoundary_t");
		CEngineServiceMgr::Get()
			->GetEventDispatcher()
			->RegisterEventListener_Abstract(
				CUtlAbstractDelegate(&Hooks::frameListener, &Hooks::FrameEventListener::OnFrameBoundary),
				"EventFrameBoundary_t",
				"D2C::OnFrameBoundary"
			);

		HookDX11Old();
	}

	// Removes any custom, non-MinHook hooks
	inline void RemoveAuxiliaryHooks() {
		CEngineServiceMgr::Get()
			->GetEventDispatcher()
			->UnregisterEventListener_Abstract(
				CUtlAbstractDelegate(&Hooks::frameListener, &Hooks::FrameEventListener::OnFrameBoundary),
				"EventFrameBoundary_t"
			);

		CEntSys::Get()->GetListeners().remove_by_value(&EntityList);
		INetworkClientService::Get()->GetIGameClient()->GetNetChannel()->UninstallMessageFilter(&d2cNetFilter);
	}

	// Reloads every hook
	inline void Reload() {
		hooks::UnhookAll();
		InstallHooks();
	}
}
