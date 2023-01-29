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

	struct SchemaParent {

		uintptr_t idk;
		ClassDescription* parent;
	};

	struct ClassDescription {

		uintptr_t idk;//0
		uintptr_t classname;//8
		uintptr_t modulename;//10
		int sizeofclass;//18
		short memberstoiterate;//1c
		char pad[6];//20
		uintptr_t MemberInfo;//28
		uintptr_t idk2;//30
		SchemaParent* parent;//38

	};


	struct SchemaTypeDescription {
		uintptr_t idk;
		uintptr_t name;
		uintptr_t idk2;
	};

	struct MemberDescription {
		uintptr_t name;
		SchemaTypeDescription* schematypeptr;
		int offset;
		int idk;
		uintptr_t idk2;
	};
	inline void SchemaDumpToMap(const char* _module, const char* _class) {
		typedef uintptr_t(__fastcall* GetScopeFn)(uintptr_t schemasys, const char* _mod);
		uintptr_t Scope = ((GetScopeFn)
			(*(uintptr_t*)(*(uintptr_t*)(Interfaces::Schema)+0x68)))(Interfaces::Schema, _module);
		if (!Scope) { return; }
		//std::cout << "Scope " << std::hex << Scope << std::dec << std::endl;
		uintptr_t Test1 = ((uintptr_t(__fastcall*)(uintptr_t scope, const char* _class))
			(*(uintptr_t*)(*(uintptr_t*)(Scope)+0x10)))(Scope, _class);
		if (!Test1) {
			//std::cout << "No such class!\n"; 
			return;
		}

		ClassDescription* a = (ClassDescription*)Test1;
	label_1: {}
		std::string className = (cc)a->classname;
		for (uintptr_t i = 0; i < a->memberstoiterate; i++) {
			MemberDescription* z = (MemberDescription*)(a->MemberInfo + i * 0x20);
			Netvars[className][(cc)z->name] = z->offset;
		}

		if (a->parent) { a = a->parent->parent; goto label_1; }
	}
}