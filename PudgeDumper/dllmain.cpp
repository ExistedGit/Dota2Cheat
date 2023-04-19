#include "pch.h"
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <span>
#include <format>
#include <chrono>
#include <array>
#include <map>
#include <set>
#include <sstream>
#include <assert.h>
#include <filesystem>
#include "sdk.h"



struct CSchemaClassBinding {
	CSchemaClassBinding* parent;
	const char* m_binary_name; // ex: C_World
	const char* m_module_name; // ex: libclient.so
	const char* m_class_name; // ex: client
	void* m_class_info_old_synthesized;
	void* m_class_info;
	void* m_this_module_binding_pointer;
	void* m_schema_type;
};

struct ClassDescription;

class CSchemaSystemTypeScope : VClass {
public:
	ClassDescription* FindDeclaredClass(const char* class_name) {
		return CallVFunc<2, ClassDescription*>(class_name);
	}

	std::string_view GetScopeName() {
		return { m_name_.data() };
	}

	CUtlTSHash<CSchemaClassBinding*> GetClasses() {
		return Member< CUtlTSHash<CSchemaClassBinding*> >(0x558);
	}

	std::array<char, 256> m_name_ = {};
};


struct SchemaParentInfo {
	uintptr_t idk;
	ClassDescription* parent;
};

struct SchemaClassFieldData_t {
	const char* m_name; // 0x0000
	SchemaTypeDescription* m_type; // 0x0008
	std::int32_t m_single_inheritance_offset; // 0x0010
	std::int32_t m_metadata_size; // 0x0014
	void* m_metadata; // 0x0018
};

struct ClassDescription {
	ClassDescription* self;        //0
	const char* className;         //8
	const char* modulename;        //10
	uint32_t classSize;               //18
	short membersSize;		   //1c
	char pad[6];				   //20
	SchemaClassFieldData_t* membersDescription; //28
	uintptr_t idk2;                //30
	SchemaParentInfo* parentInfo;  //38
};


struct SchemaTypeDescription {
	uintptr_t idk;
	const char* name;
	uintptr_t idk2;
};

struct less_than_key
{
	inline bool operator() (const SchemaClassFieldData_t& struct1, const SchemaClassFieldData_t& struct2) const
	{
		return (struct1.m_single_inheritance_offset < struct2.m_single_inheritance_offset);
	}
};

std::unordered_map<std::string, std::set<SchemaClassFieldData_t, less_than_key>> Netvars;

typedef void* (*oCreateInterface)(const char*, int);
oCreateInterface pCreateInterface;
uintptr_t CreateInterface(const char* szModule, const char* szInterface) {
	pCreateInterface = (oCreateInterface)GetProcAddress(GetModuleHandleA(szModule), "CreateInterface");
	return (uintptr_t)pCreateInterface(szInterface, 0);
}

class CSchemaSystem : public VClass {
public:
	CSchemaSystemTypeScope* GlobalTypeScope(void) {
		return CallVFunc<11, CSchemaSystemTypeScope*>();
	}

	CSchemaSystemTypeScope* FindTypeScopeForModule(const char* m_module_name) {
		return CallVFunc<13, CSchemaSystemTypeScope*>(m_module_name);
	}

	CUtlVector<CSchemaSystemTypeScope*> GetTypeScopes() {
		return Member<CUtlVector<CSchemaSystemTypeScope*>>(0x190);
	}
};

CSchemaSystem* SchemaSystem = 0;

inline void DumpClassMembers(ClassDescription* classDesc) {
	std::cout << "Dumping " << classDesc->className << "...\n";

	std::string className = classDesc->className;
	for (uintptr_t i = 0; i < classDesc->membersSize; i++) {
		SchemaClassFieldData_t desc = classDesc->membersDescription[i];
		Netvars[className].insert(desc);
		//		std::cout << std::format("{}: {} ({})\n", info.schematypeptr->name, info.name, info.offset);
	}

	if (classDesc->parentInfo) {
		classDesc = classDesc->parentInfo->parent;
		DumpClassMembers(classDesc);
	}

}

template<typename... Args>
void SchemaDumpToMap(const char* _module, Args&&... args) {
	const char* classes[sizeof...(args)] = { std::forward<Args>(args)... };
	auto typeScope = SchemaSystem->CallVFunc<13, CSchemaSystemTypeScope*>(_module);

	if (!typeScope)
		return;

	for (auto& _class : classes) {
		//std::cout << "Scope " << std::hex << Scope << std::dec << std::endl;
		auto classDesc = typeScope->FindDeclaredClass(_class);
		if (!classDesc) {
			std::cout << "No such class: " << _class << "\n";
			return;
		}
		DumpClassMembers(classDesc);
	}
}

std::string getTimeStr() {
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::string s(30, '\0');
	std::strftime(&s[0], s.size(), "%d-%m-%Y %H:%M:%S", std::localtime(&now));
	return s;
}

void DumpAllClasses(std::string& dir) {
	if (!std::filesystem::exists(dir)) {
		std::cout << "No such directory: " << dir << '\n';
		return;
	}

	auto scopes = SchemaSystem->GetTypeScopes();
	for (auto scope : scopes) {
		auto classes = scope->GetClasses();
		std::filesystem::create_directory(dir + "\\" + scope->GetScopeName().data());

		for (const auto _class : classes.GetElements()) {
			const auto classDesc = scope->FindDeclaredClass(_class->m_binary_name);

			for (auto i = 0; i < classDesc->membersSize; i++) {
				auto memberDesc = &classDesc->membersDescription[i];
				if (!memberDesc)
					continue;

			}
		}
	}
}

void SaveNetvarsToFile(std::ofstream& fout) {
	fout << std::hex;
	fout << "#pragma once\n#include <cstdint>\nnamespace Netvars {\n";
	for (auto& [className, classMap] : Netvars) {
		fout << std::format("\tnamespace {}", className) << " {\n";
		for (auto& desc : classMap) {
			fout << std::format("\t\tconstexpr uint32_t {} = ", desc.m_name) << "0x" << desc.m_single_inheritance_offset << ";";
			fout << " // " << desc.m_type << '\n';
		}
		fout << "\t}\n";
	}
	fout << "}";
}

uintptr_t WINAPI HackThread(HMODULE hModule) {
	const bool console = true;

	FILE* f;
	if (console) {
		AllocConsole();
		freopen_s(&f, "CONOUT$", "w", stdout);
	}


	SchemaSystem = (CSchemaSystem*)CreateInterface("schemasystem.dll", "SchemaSystem_001");
	std::cout << "Dumping...\n";


	std::ofstream fout("C:\\Users\\user\\Documents\\schema_dump.txt");
	fout << "Dump started at " << getTimeStr() << std::endl << std::endl;

	clock_t timeStart = clock();
	DumpAllClasses(fout);

	clock_t timeEnd = clock();

	fout << "\nTime elapsed " << round(((double)(timeEnd - timeStart) / CLOCKS_PER_SEC) * 10) / 10 << "s" << '\n';
	fout << "Dumped at " << getTimeStr() << '\n';

	fout.close();

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

	std::ofstream fout("H:\\Reversing\\SchemaDump\\header.h");
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

