#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	// The client stores convars directly by pointers, whereas the server would query them by name
	// We disguise the original convar and present the server with the "original", "untouched" dummy
	inline class M_CVarSpoofer {

		// Reversed by looking at various calls to RegisterConVar and CCVar's 35th vfunc
		// Not sure if all of it is correct
#pragma pack(push, 1)
		struct CVarInitInfo {
			PAD(4);
			bool idk = true;
			PAD(2);
			union {
				float m_flDefVal;
				uint32_t m_iDefVal;
				uint64_t m_pDefVal;
			};
			PAD(1);
			PAD(8 * 5);
			void(*callback)() = nullptr;
			uint64_t type;
		};
#pragma pack(pop)
		struct CVarID2 : public CVarID {
			uint32_t id2 = 0xFFFFFFFF;
		};

		struct RegisterConVarInfo {
			const char
				* name, * desc;
			uint64_t flags;
			CVarInitInfo info;
			CVarID2* extendedId;
			void* id;
		};

		static_assert(sizeof(CVarInitInfo) == 0x48);

		struct SpoofedCVarData {
			CVar* original;
			uint64_t impl;
		};

		std::map<CVar*, SpoofedCVarData> spoofedVars;
		void SpoofVar(const char* varName);
	public:
		template<typename ...Args>
		void SpoofVars(Args&&... varNamePack) {
			const char* varNames[sizeof...(varNamePack)] = { std::forward<Args>(varNamePack)... };
			for (auto varName : varNames)
				SpoofVar(varName);
		}
		void RestoreVars();
	} CVarSpoofer{};
}