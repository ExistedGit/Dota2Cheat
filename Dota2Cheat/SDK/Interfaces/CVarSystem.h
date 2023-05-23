#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <map>
#include <span>
#include <array>
#include "../Base/Definitions.h"
#include "../Base/VClass.h"

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
	std::array<float, 2> two_floats;
	std::array<float, 3> three_floats;
	std::array<float, 4> four_floats;
};

struct CVar {
	const char* name{};
	CVar* m_pNext{};
	uintptr_t unk1{};
	uintptr_t unk2{};
	const char* desc{};
	EConvarType type{};
	int unk_maybe_number_of_times_changed{};
	int flags{};
	int unk4{};
	int m_iCallbackIndex{};
	int unk5{};
	CVarValue value{};
	CVarValue defaultValue{};
};

// Element of the convar list
struct CVarNode {
	CVar* var;
	void* unk;
};

// Used for callbacks
struct CVarID
{
	static inline constexpr auto BAD_ID = 0xFFFFFFFF;
	std::uint64_t impl{};
	CVar* m_pVar{};

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

	static inline std::map<std::string, CVarID> CVars{};
	using CVarCallbackFn = void(*)(const CVarID& id, int unk1, const CVarValue* val, const CVarValue* old_val);

	CVarCallbackFn GetCallback(int id) {
		return *(CVarCallbackFn*)(Member<uintptr_t>(0x80) + 24 * id);
	}

	void TriggerCallback(const CVarID& id) {
		const auto cvar = id.m_pVar;
		GetCallback(cvar->m_iCallbackIndex)(id, 0, &cvar->value, &cvar->value);
	}

	GETTER(uint32_t, GetCVarCount, 0x58);
	GETTER(CVarNode*, GetCVarNodeList, 0x40);

	void DumpConVarsToMap() {
		auto list = GetCVarNodeList();
		auto size = GetCVarCount();
		for (int i = 0; i < size; i++) {
			auto var = list[i].var;
			if (!var || !var->name)
				continue;

			CVarID id;
			id.m_pVar = var;
			id.impl = static_cast<uint64_t>(i);
			CVars[var->name] = id;
		}
	}
};
