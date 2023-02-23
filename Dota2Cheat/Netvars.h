#pragma once
#include <cstdint>

#define Offset constexpr uint32_t

namespace Netvars {
	namespace CDOTAModifier {
		Offset m_name = 0x28;
		Offset m_flDuration = 0x70;
		Offset m_flDieTime = 0x74;
		Offset m_hCaster = 0x78;
		Offset m_hAbility = 0x7c;
		Offset m_hParent = 0x80;
	}
}

#undef Offset