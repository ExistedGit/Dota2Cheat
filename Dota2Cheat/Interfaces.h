#pragma once
//#include "SDK/IEngineClient.h"
//#include "SDK/CGameEntitySystem.h"
#include "SDK/CSource2Client.h"
#include "SDK/CNetworkMessages.h"
#include "sdk.h"
#include "CGameEntitySystem.h"
#include "ConVar.h"
#include "IEngineClient.h"
#include "CInputService.h"
#include "CBaseFileSystem.h"
#include "CGCClient.h"


namespace Interfaces {
	inline CVarSystem* CVar = nullptr;
	inline IEngineClient* Engine = nullptr;
	inline VClass* Panorama = nullptr;
	inline VClass* UIEngine = nullptr;
	inline CSource2Client* Client = nullptr;
	inline CGameEntitySystem* EntitySystem = nullptr;
	inline CGCClient* GCClient = nullptr;
	inline CInputService* InputService = nullptr;
	inline VClass* Schema = nullptr;
	inline CBaseFileSystem* FileSystem = nullptr;
	inline void* NetworkSystem;
	inline CNetworkMessages* NetworkMessages = nullptr;

	typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);
	template<typename T>
	inline T GetInterface(const char* dllName, const char* interfaceName) {
		tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandleA(dllName), "CreateInterface");
		int retCode = 0;
		void* retInterface = CreateInterface(interfaceName, &retCode);
		return reinterpret_cast<T>(retInterface);
	}


	inline void InitInterfaces() {
		Engine = GetInterface<IEngineClient*>("engine2.dll", "Source2EngineToClient001");
		Client = GetInterface<CSource2Client*>("client.dll", "Source2Client002");
		CVar = GetInterface<CVarSystem*>("tier0.dll", "VEngineCvar007");

		uintptr_t* vmt_slot = *(uintptr_t**)Interfaces::Client + 25;								//25th function in Source2Client vtable
		uintptr_t addr_start = *vmt_slot + 3;														//stores the relative address portion of the mov rax, [rip + 0x2512059] instruction
		EntitySystem = *(CGameEntitySystem**)(addr_start + *(uint32_t*)(addr_start)+4);

		FileSystem = GetInterface<CBaseFileSystem*>("filesystem_stdio.dll", "VFileSystem017");
		//auto file = FileSystem->OpenFile("scripts/npc/npc_heroes.txt", "r");
		//char buffer[256];
		//FileSystem->ReadLine(buffer, 256, file);

		Panorama = GetInterface<VClass*>("panorama.dll", "PanoramaUIEngine001");
		UIEngine = Panorama->Member<VClass*>(0x28);

		GCClient = GetInterface<CGCClient*>("client.dll", "DOTA_CLIENT_GCCLIENT");
		Schema = GetInterface<VClass*>("schemasystem.dll", "SchemaSystem_001");
		InputService = GetInterface<CInputService*>("engine2.dll", "InputService_001");
		NetworkSystem = GetInterface<void*>("networksystem.dll", "NetworkSystemVersion001");
		NetworkMessages = GetInterface<CNetworkMessages*>("networksystem.dll", "NetworkMessagesVersion001");
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