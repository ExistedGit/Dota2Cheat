#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	// The client stores convars directly by pointers, whereas the server would query them by name
	// We disguise the original convar and present the server with the "original", "untouched" dummy
	inline class M_CVarSpoofer {

#pragma pack(push, 1)
		struct CVarRegInfo {
			PAD(4);
			bool idk = true;
			PAD(2);
			union {
				float m_flDefVal;
				uint32_t m_iDefVal;
				uint64_t m_pDefVal;
			};
			PAD(8 * 6);
			bool unk5;
			char type;
			PAD(7);
		};
#pragma pack(pop)

		static_assert(sizeof(CVarRegInfo) == 72);

		// map of dummy vars to their original versions
		// used for swapping names back at revert
		std::map<CVar*, CVar*> originalVars;
		void SpoofVar(const char* varName);
	public:
		template<typename ...Args>
		void SpoofVars(Args&&... varNamePack) {
			const char* varNames[sizeof...(varNamePack)] = { std::forward<Args>(varNamePack)... };
			for (auto varName : varNames)
				SpoofVar(varName);
		}
	} CVarSpoofer{};
}