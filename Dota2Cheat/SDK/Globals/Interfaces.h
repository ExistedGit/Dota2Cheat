#pragma once

#include "../Interfaces/CGameEntitySystem.h"
#include "../Interfaces/IEngineClient.h"
#include "../Interfaces/CInputService.h"
#include "../Interfaces/CBaseFileSystem.h"
#include "../Interfaces/Network/CNetworkMessages.h"
#include "../Interfaces/CGCClient.h"
#include "../Interfaces/CVarSystem.h"
#include "../Interfaces/Panorama.h"
#include "../Base/Memory.h"

namespace Interfaces {
	inline CVarSystem* CVar{};
	inline IEngineClient* Engine{};
	inline VClass* Panorama{};
	inline Panorama::CUIEngineSource2* UIEngine{};
	inline VClass* Client{};
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

	inline void FindInterfaces() {
		Engine = GetInterface<IEngineClient>("engine2.dll", "Source2EngineToClient001");
		Client = GetInterface<VClass>("client.dll", "Source2Client002");
		CVar = GetInterface<CVarSystem>("tier0.dll", "VEngineCvar007");

		EntitySystem = *GetAbsoluteAddress<CGameEntitySystem**>((uintptr_t)Interfaces::Client->GetVFunc(25).ptr, 3, 7);

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