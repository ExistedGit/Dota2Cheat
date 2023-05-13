#pragma once
#include <cstdint>
#include "../Globals/Interfaces.h"
template<typename T = CBaseEntity>
struct CHandle {
	uint32_t val;
	operator uint32_t() {
		return val;
	}
	uint32_t Index() {
		return val & 0x7fff;
	}
	T* Entity() {
		return Interfaces::EntitySystem->GetEntity<T>(Index());
	}
	operator T* () {
		return Entity();
	}
};