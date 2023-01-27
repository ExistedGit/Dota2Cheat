#pragma once
//#include "SDK/IEngineClient.h"
//#include "SDK/CGameEntitySystem.h"
#include "SDK/CSource2Client.h"
#include "sdk.h"
#include "CGameEntitySystem.h"
#include "ConVar.h"
#include "IEngineClient.h"

namespace Interfaces {
	inline CVarSystem* CVar = nullptr;
	inline IEngineClient* Engine =  nullptr;
	inline VClass* Panorama = nullptr;
	inline VClass* Panorama2 = nullptr;
	inline CSource2Client* Client = nullptr;
	inline CGameEntitySystem* Entity = nullptr;
	
	inline void* GCClient = nullptr;
	inline uintptr_t Schema = NULL;

	typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);
	template<typename T>
	inline  T GetInterface(const char* dllName, const char* interfaceName) {
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
		Entity = *(CGameEntitySystem**)(addr_start + *(uint32_t*)(addr_start)+4);

		
		Panorama = GetInterface<VClass*>("panorama.dll", "PanoramaUIEngine001");
		Panorama2 = Panorama->Member<VClass*>(0x28);
		GCClient = GetInterface<void*>("client.dll", "DOTA_CLIENT_GCCLIENT");
		Schema = GetInterface<uintptr_t>("schemasystem.dll", "SchemaSystem_001");
	}
	inline  void LogInterfaces() {
		std::cout << "[INTERFACES]\n" << std::hex;
		std::cout << "Schema: " << Interfaces::Schema << "\n";
		std::cout << "EntitySystem: " << Interfaces::Entity << "\n";
		std::cout << "IEngineClient: " << Interfaces::Engine << "\n";
		std::cout << "Source2Client: " << Interfaces::Client << "\n";
		std::cout << "CVarSystem: " << Interfaces::CVar << "\n";
		std::cout << "Panorama: " << Interfaces::Panorama << "\n";
		std::cout << "Panorama2: " << Interfaces::Panorama2 << "\n";
		//std::cout << std::dec;
		std::cout << "GCClient: " << Interfaces::GCClient << "\n";
	}
}