#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

#include "../Base/VClass.h"
//структуры получены путем наблюдения глазиками в реклассе/дебагере той фигни которую вернула функция FindDeclaredClass, если интересно сделайте у себя там просто CMSG с возвратом этой функции и посмотрите в реклассе/дебагере
//EN: structures were reversed by observing the object FindDeclaredClass returns in ReClass/x64dbg, if you're interested make a CMSG with what it returns and look into it in ReClass/x64dbg
struct ClassDescription;
struct MemberDescription;

struct SchemaParentInfo {
	uintptr_t idk;
	ClassDescription* parent;
};


struct ClassDescription {

	uintptr_t idk;//0
	const char* className;//8
	const char* modulename;//10
	int sizeofclass;//18
	short membersToIterate;//1c
	char pad[6];//20
	MemberDescription* MemberInfo;//28
	uintptr_t idk2;//30
	SchemaParentInfo* parentInfo;//38
};


struct SchemaTypeDescription {
	uintptr_t idk;
	const char* name;
	uintptr_t idk2;
};

struct MemberDescription {
	const char* name;
	SchemaTypeDescription* schematypeptr;
	int offset;
	int idk;
	uintptr_t idk2;
};

// You can use these to update Netvars.h

//inline void DumpClassMembers(ClassDescription* classDesc) {
//	std::string className = classDesc->className;
//	for (uintptr_t i = 0; i < classDesc->membersToIterate; i++) {
//		MemberDescription desc = classDesc->MemberInfo[i];
//		Netvars[className][desc.name] = desc;
//		//		std::cout << std::format("{}: {} ({})\n", info.schematypeptr->name, info.name, info.offset);
//	}

//	if (classDesc->parentInfo) {
//		classDesc = classDesc->parentInfo->parent;
//		DumpClassMembers(classDesc);
//	}

//}
//inline void SchemaDumpToMap(const char* _module, const char* _class) {
//	auto Scope = SchemaSystem->CallVFunc<13, VClass*>(_module);
//	if (!Scope) { return; }
//	//std::cout << "Scope " << std::hex << Scope << std::dec << std::endl;
//	auto classDesc = Scope->CallVFunc<2, ClassDescription*>(_class);
//	if (!classDesc) {
//		//std::cout << "No such class!\n"; 
//		return;
//	}
//	DumpClassMembers(classDesc);
//}

//inline void DumpNetvarsToFile() {
//	std::ofstream fout("H:\\Reversing\\SchemaDump\\header.h");
//	fout << std::hex;
//	fout << "#pragma once\n#include <cstdint>\nnamespace Netvars {\n";
//	for (auto& [className, classMap] : Netvars) {
//		fout << std::format("\tnamespace {}", className) << " {\n";
//		for (auto& [netvar, desc] : classMap) {
//			fout << std::format("\t\tconstexpr uint32_t {} = ", netvar) << "0x" << desc.offset << ";";
//			fout << " // " << desc.schematypeptr->name << '\n';
//		}
//		fout << "\t}\n";
//	}
//	fout << "}";
//}
