#pragma once
//#include "SDK/IEngineClient.h"
//#include "SDK/CGameEntitySystem.h"
#include "SDK/CSource2Client.h"
#include "sdk.h"
#include "CGameEntitySystem.h"
#include "ConVar.h"

namespace Interfaces {
	CVarSystem* CVar;
	void* Engine;
	VClass* Panorama;
	void* Panorama2;
	CSource2Client* Client;
	CGameEntitySystem* Entity;
	uintptr_t Schema;

	typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);
	template<typename T>
	T GetInterface(const char* dllName, const char* interfaceName) {
		tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandleA(dllName), "CreateInterface");
		int retCode = 0;
		void* retInterface = CreateInterface(interfaceName, &retCode);
		return reinterpret_cast<T>(retInterface);
	}
	
	void LogInterfaces() {
		std::cout << std::hex;
		std::cout << "Schema: " << Interfaces::Schema << "\n";
		std::cout << "EntitySystem: " << Interfaces::Entity << "\n";
		std::cout << "IEngineClient: " << Interfaces::Engine << "\n";
		std::cout << "Source2Client: " << Interfaces::Client << "\n";
		std::cout << "CVarSystem: " << Interfaces::CVar << "\n";
		std::cout << "Panorama: " << Interfaces::Panorama << "\n";
		std::cout << "Panorama2: " << Interfaces::Panorama2 << "\n";
		//std::cout << std::dec;
	}
	void InitInterfaces() {
		Engine = GetInterface<void*>("engine2.dll", "Source2EngineToClient001");
		Client = GetInterface<CSource2Client*>("client.dll", "Source2Client002");
		CVar = GetInterface<CVarSystem*>("tier0.dll", "VEngineCvar007");
		uintptr_t* vmt_slot = *(uintptr_t**)Interfaces::Client + 25;								//25th function in Source2Client vtable
		uintptr_t addr_start = *vmt_slot + 3;														//stores the relative address portion of the mov rax, [rip + 0x2512059] instruction
		Entity = *(CGameEntitySystem**)(addr_start + *(uint32_t*)(addr_start)+4);


		Panorama = GetInterface<VClass*>("panorama.dll", "PanoramaUIEngine001");
		Panorama2 = Panorama->Member<void*>(0x28);

		Schema = GetInterface<uintptr_t>("schemasystem.dll", "SchemaSystem_001");
	}
}