#pragma once
#include <cstdint>
#include "../../CheatSDK/Globals/Interfaces.h"

template<typename T = CBaseEntity>
struct CHandle {
	constexpr static uint32_t INVALID_HANDLE = 0XFFFFFFFF;

	uint32_t val = INVALID_HANDLE;
	operator uint32_t() const {
		return val;
	}
	uint32_t Index() const {
		return val & 0x7fff;
	}
	T* Entity() const {
		return Interfaces::EntitySystem->GetEntity<T>(Index());
	}

	operator T* () const {
		return Entity();
	}

	T* operator->() const {
		return Entity();
	}

	T* operator*() const {
		return Entity();
	}

	bool IsValid() const {
		return val != INVALID_HANDLE;
	}

	bool operator==(const CHandle<T> other) const {
		return other.val == val;
	}

	CHandle<T>& operator=(uint32_t val) {
		this->val = val;
		return *this;
	}
};

