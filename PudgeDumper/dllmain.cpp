#include "pch.h"
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <span>
#include "sdk.h"
#include <format>
#include <map>
#include <set>

struct ClassDescription;
struct MemberDescription;

struct SchemaParentInfo {
	uintptr_t idk;
	ClassDescription* parent;
};

struct ClassDescription {
	ClassDescription* self;        //0
	const char* className;         //8
	const char* modulename;        //10
	uint32_t classSize;               //18
	short membersSize;		   //1c
	char pad[6];				   //20
	MemberDescription* membersDescription; //28
	uintptr_t idk2;                //30
	SchemaParentInfo* parentInfo;  //38
};


struct SchemaTypeDescription {
	uintptr_t idk;
	const char* name;
	uintptr_t idk2;
};

struct MemberDescription {
	const char* name;
	SchemaTypeDescription* typeDesc;
	int offset;
	int idk;
	uintptr_t idk2;
};
struct less_than_key
{
	inline bool operator() (const MemberDescription& struct1, const MemberDescription& struct2) const
	{
		return (struct1.offset < struct2.offset);
	}
};

std::unordered_map<std::string, std::set<MemberDescription, less_than_key>> Netvars;

typedef void* (*oCreateInterface)(const char*, int);
oCreateInterface pCreateInterface;
uintptr_t CreateInterface(const char* szModule, const char* szInterface) {
	pCreateInterface = (oCreateInterface)GetProcAddress(GetModuleHandleA(szModule), "CreateInterface");
	return (uintptr_t)pCreateInterface(szInterface, 0);
}

VClass* SchemaSystem = 0;

inline void DumpClassMembers(ClassDescription* classDesc) {
	std::cout << "Dumping " << classDesc->className << "...\n";

	std::string className = classDesc->className;
	for (uintptr_t i = 0; i < classDesc->membersSize; i++) {
		MemberDescription desc = classDesc->membersDescription[i];
		Netvars[className].insert(desc);
		//		std::cout << std::format("{}: {} ({})\n", info.schematypeptr->name, info.name, info.offset);
	}

	if (classDesc->parentInfo) {
		classDesc = classDesc->parentInfo->parent;
		DumpClassMembers(classDesc);
	}

}

template<typename... Args>
inline void SchemaDumpToMap(const char* _module, Args&&... args) {
	const char* classes[sizeof...(args)] = {std::forward<Args>(args)...};
	auto typeScope = SchemaSystem->CallVFunc<13, VClass*>(_module);

	if (!typeScope)
		return;

	for (auto& _class : classes) {
		//std::cout << "Scope " << std::hex << Scope << std::dec << std::endl;
		auto classDesc = typeScope->CallVFunc<2, ClassDescription*>(_class);
		if (!classDesc) {
			std::cout << "No such class: " << _class << "\n";
			return;
		}
		DumpClassMembers(classDesc);
	}
}

uintptr_t WINAPI HackThread(HMODULE hModule) {
	const bool console = true;

	FILE* f;
	if (console) {
		AllocConsole();
		freopen_s(&f, "CONOUT$", "w", stdout);
	}


	SchemaSystem = (VClass*)CreateInterface("schemasystem.dll", "SchemaSystem_001");
	std::cout << "Dumping...\n";
	// You can add some other classes there if you need to
	SchemaDumpToMap("client.dll",
		"CEntityIdentity",
		"C_DOTA_Item",
		"C_DOTA_BaseNPC_Hero",
		"C_DOTAPlayerController",
		"C_DOTA_UnitInventory",
		"C_DOTABaseAbility",
		"C_DOTA_PlayerResource",
		"PlayerResourcePlayerTeamData_t",
		"PlayerResourcePlayerData_t",
		"C_DOTAGamerules",
		"CGameSceneNode",
		"C_DOTA_Item_Rune",
		"GameTime_t",
		"C_DOTA_Item_EmptyBottle");

	SchemaDumpToMap("server.dll", "CDOTA_Buff");

	// Replace this path with something valid
	// Probably will do some FileDialog thing in the future
	std::ofstream fout("H:\\Reversing\\SchemaDump\\header.h");
	fout << std::hex;
	fout << "#pragma once\n#include <cstdint>\nnamespace Netvars {\n";
	for (auto& [className, classMap] : Netvars) {
		fout << std::format("\tnamespace {}", className) << " {\n";
		for (auto& desc : classMap) {
			fout << std::format("\t\tconstexpr uint32_t {} = ", desc.name) << "0x" << desc.offset << ";";
			fout << " // " << desc.typeDesc->name << '\n';
		}
		fout << "\t}\n";
	}
	fout << "}";
	std::cout << "Done!\n";

	if (console) {
		system("pause");
		if (f) fclose(f);
		FreeConsole();
	}
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {

		HANDLE handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
		if (handle) CloseHandle(handle);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

