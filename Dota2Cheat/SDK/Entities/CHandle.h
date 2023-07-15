#pragma once
#include <cstdint>
#include "../../CheatSDK/Globals/Interfaces.h"

template<typename T = CBaseEntity>
struct CHandle {
	constexpr static uint32_t INVALID_HANDLE = 0XFFFFFFFF;

	uint32_t val = INVALID_HANDLE;
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

	T* operator->() {
		return Entity();
	}

	bool IsValid() {
		return val != INVALID_HANDLE;
	}

	bool operator==(const CHandle<T> other) {
		return other.val == val;
	}

	CHandle<T>& operator=(uint32_t val) {
		this->val = val;
		return *this;
	}
};

