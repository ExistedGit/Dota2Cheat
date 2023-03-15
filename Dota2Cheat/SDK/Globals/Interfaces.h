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
	inline CEngineClient* Engine{};
	inline VClass* Panorama{};
	inline Panorama::CUIEngineSource2* UIEngine{};
	inline VClass* Client{};
	inline CGameEntitySystem* EntitySystem{};
	inline CGCClient* GCClient{};
	inline CInputService* InputService{};
	inline VClass* Schema{};
	inline VClass* ParticleMgrSystem{};
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

	void FindInterfaces();
	void LogInterfaces();
}