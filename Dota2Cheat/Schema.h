#pragma once
#include <unordered_map>
#include <Psapi.h>
#include <cstdint>
#include "Interfaces.h"
#include <iostream>
#include <string>
#include <unordered_map>

namespace Schema {

	inline std::unordered_map<std::string, std::unordered_map<std::string, uintptr_t>> Netvars;

	//структуры получены путем наблюдения глазиками в реклассе/дебагере той фигни которую вернула функция FindDeclaredClass, если интересно сделайте у себя там просто CMSG с возвратом этой функции и посмотрите в реклассе/дебагере
	struct ClassDescription;

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
		uintptr_t MemberInfo;//28
		uintptr_t idk2;//30
		SchemaParentInfo* parentInfo;//38

	};


	struct SchemaTypeDescription {
		uintptr_t idk;
		uintptr_t name;
		uintptr_t idk2;
	};

	struct MemberDescription {
		const char* name;
		SchemaTypeDescription* schematypeptr;
		int offset;
		int idk;
		uintptr_t idk2;
	};
	inline void DumpClassMembers(ClassDescription* classDesc) {
		std::string className = classDesc->className;
		for (uintptr_t i = 0; i < classDesc->membersToIterate; i++) {
			MemberDescription* member = (MemberDescription*)(classDesc->MemberInfo + i * 0x20);
			Netvars[className][member->name] = member->offset;
		}

		if (classDesc->parentInfo) { 
			classDesc = classDesc->parentInfo->parent;
			DumpClassMembers(classDesc);
		}

	}
	inline void SchemaDumpToMap(const char* _module, const char* _class) {
		auto Scope = Interfaces::Schema->CallVFunc<13, VClass*>(_module);
		if (!Scope) { return; }

		//std::cout << "Scope " << std::hex << Scope << std::dec << std::endl;
		ClassDescription* classDesc = Scope->CallVFunc<2, ClassDescription*>(_class);
		if (!classDesc) {
			//std::cout << "No such class!\n"; 
			return;
		}
		DumpClassMembers(classDesc);
	}
}