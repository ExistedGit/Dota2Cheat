#pragma once

#include "sdk.h"
#include "SDK/CSource2Client.h"
#include "SDK/CNetworkMessages.h"
#include "CGameEntitySystem.h"
#include "IEngineClient.h"
#include "CInputService.h"
#include "CBaseFileSystem.h"
#include "CGCClient.h"
#include "ConVar.h"
#include "Panorama.h"

namespace Interfaces {
	inline CVarSystem* CVar{};
	inline IEngineClient* Engine{};
	inline VClass* Panorama{};
	inline Panorama::CUIEngineSource2* UIEngine{};
	inline CSource2Client* Client{};
	inline CGameEntitySystem* EntitySystem{};
	inline CGCClient* GCClient{};
	inline CInputService* InputService{};
	inline VClass* Schema{};
	inline CBaseFileSystem* FileSystem{};
	inline void* NetworkSystem;
	inline CNetworkMessages* NetworkMessages{};

	typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);
	template<typename T>
	inline T* GetInterface(const char* dllName, const char* interfaceName) {
		tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandleA(dllName), "CreateInterface");
		int retCode = 0;
		void* retInterface = CreateInterface(interfaceName, &retCode);
		return reinterpret_cast<T*>(retInterface);
	}

	inline void InitInterfaces() {
		Engine = GetInterface<IEngineClient>("engine2.dll", "Source2EngineToClient001");
		Client = GetInterface<CSource2Client>("client.dll", "Source2Client002");
		CVar = GetInterface<CVarSystem>("tier0.dll", "VEngineCvar007");

		uintptr_t* vmt_slot = *(uintptr_t**)Interfaces::Client + 25;								//25th function in Source2Client vtable
		uintptr_t addr_start = *vmt_slot + 3;														//stores the relative address portion of the mov rax, [rip + 0x2512059] instruction
		EntitySystem = *(CGameEntitySystem**)(addr_start + *(uint32_t*)(addr_start)+4);

		FileSystem = GetInterface<CBaseFileSystem>("filesystem_stdio.dll", "VFileSystem017");

		Panorama = GetInterface<VClass>("panorama.dll", "PanoramaUIEngine001");
		UIEngine = Panorama->Member<Panorama::CUIEngineSource2*>(0x28);

		GCClient = GetInterface<CGCClient>("client.dll", "DOTA_CLIENT_GCCLIENT");
		Schema = GetInterface<VClass>("schemasystem.dll", "SchemaSystem_001");
		InputService = GetInterface<CInputService>("engine2.dll", "InputService_001");
		NetworkSystem = GetInterface<void>("networksystem.dll", "NetworkSystemVersion001");
		NetworkMessages = GetInterface<CNetworkMessages>("networksystem.dll", "NetworkMessagesVersion001");
	}
	inline  void LogInterfaces() {
		std::cout << "[INTERFACES]\n";
		std::cout << "Schema: " << Interfaces::Schema << "\n";
		std::cout << "EntitySystem: " << Interfaces::EntitySystem << "\n";
		std::cout << "IEngineClient: " << Interfaces::Engine << "\n";
		std::cout << "Source2Client: " << Interfaces::Client << "\n";
		std::cout << "CVarSystem: " << Interfaces::CVar << "\n";
		std::cout << "Panorama: " << Interfaces::Panorama << "\n";
		std::cout << "Panorama2: " << Interfaces::UIEngine << "\n";
		std::cout << "GCClient: " << Interfaces::GCClient << "\n";
	}
}