#pragma once
#include <cstdint>

class CBaseEntity;

template<typename T = CBaseEntity>
struct CHandle {
	constexpr static uint32_t INVALID_HANDLE = 0XFFFFFFFF;

	uint32_t val;

	CHandle() : val(INVALID_HANDLE) {}
	CHandle(uint32_t val) : val(val) {}

	uint32_t Index() const {
		return val & 0x7fff;
	}

	explicit operator uint32_t() const {
		return val;
	}

	explicit operator bool() const {
		return IsValid();
	}

	T* Entity() const;

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
};

template<typename T>
std::ostream& operator<<(std::ostream& os, CHandle<T> h) {
	return os << "H[" << (*h) << "]";
}