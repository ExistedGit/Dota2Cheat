#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <map>
#include "../Base/VClass.h"
#include <span>

#define MAX_CVAR_COUNT 4280

enum class EConvarType : std::uint8_t
{
	BOOL = 0,
	INT32,
	UINT32,
	INT64,
	UINT64,
	FLOAT,
	DOUBLE,
	STRING,
	COLOR_RGBA,
	UNK_SOME_TWO_FLOATS,
	UNK_SOME_THREE_FLOATS,
	UNK_SOME_FOUR_FLOATS,
	UNK_SOME_THREE_FLOATS_AGAIN,
};
union CVarValue
{
	bool boolean{};
	std::uint64_t ui64;
	std::int64_t i64;
	std::uint32_t ui32;
	std::int32_t i32;
	float flt;
	double dbl;
	const char* str;
	std::uint32_t clr_rgba;
	//std::array<float, 2> two_floats;
	//std::array<float, 3> three_floats;
	//std::array<float, 4> four_floats;
};

struct CVar {
	const char* name{};
	CVar* m_Pnext{};
	uintptr_t unk1{};
	uintptr_t unk2{};
	const char* desc{};
	EConvarType type{};
	int unk_maybe_number_of_times_changed{};
	int flags{};
	int unk4{};
	int CALLBACK_INDEX{};
	int unk5{};
	CVarValue value{};
	CVarValue defaultValue{};
};
struct CVarNode {
	CVar* var;
	void* unk;
};

struct CVarID
{
	static inline constexpr auto BAD_ID = 0xFFFFFFFF;
	std::uint64_t impl{};
	void* var_ptr{};

	bool IsGood() const noexcept
	{
		return impl != BAD_ID;
	}

	void Invalidate() noexcept
	{
		impl = BAD_ID;
	}
};
class CVarSystem : VClass {
public:
	struct CVarInfo {
		CVar* var{};
		int index{};
	};

	//static std::map<ConVar*, int> CVar;
	static inline std::map<std::string, CVarInfo> CVar{};
	using CVarCallbackFn = void(*)(const CVarID& id, int unk1, const CVarValue* val, const CVarValue* old_val);

	CVarCallbackFn GetCallback(int id) {
		return *(CVarCallbackFn*)(Member<uintptr_t>(0x80) + 24 * id);
	}
	void TriggerCallback(CVarInfo info) {
		const auto old_val = info.var->value;
		CVarID cvid;
		cvid.impl = static_cast<uint64_t>(info.index);
		cvid.var_ptr = info.var;
		GetCallback(info.var->CALLBACK_INDEX)(cvid, 0, &info.var->value, &old_val);
	}
	uint32_t GetCVarCount() {
		return Member<uint32_t>(0x58);
	}
	auto GetCVarNodeList() {
		return std::span<CVarNode, MAX_CVAR_COUNT>{ Member<CVarNode*>(0x40), MAX_CVAR_COUNT };
	};
	void DumpConVarsToMap() {
		auto list = GetCVarNodeList();
		for (int i = 0; i < list.size(); i++) {
			auto var = list[i].var;
			if (var  &&
				var->name ) {
				CVarInfo info;
				info.var = list[i].var;
				info.index = i;

				CVar[var->name] = info;
			}
		}
	}
	void DumpConVarsToFile(const char* path) {
		std::ofstream file{ path };
		if (file.is_open()) {
			auto list = GetCVarNodeList();
			for (int i = 0; i < list.size(); i++) {
				if (list[i].var  &&
					list[i].var->name )
					file << list[i].var->name << " " << i << '\n';
			}

			file.close();
		}
	}
	void SetConvars() {
		CVar["sv_cheats"].var->value.boolean = true;
		CVar["r_farz"].var->value.flt = 10000.0f;
		CVar["fog_enable"].var->value.boolean = false;
	}

};
