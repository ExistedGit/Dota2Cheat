#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <map>
#include <span>
#include <array>
#include "../Base/Definitions.h"
#include "../Base/VClass.h"

// defines from TF2 sources. All hail the Source Engine!
#define FCVAR_CHEAT	(1<<14)

#define FCVAR_UNREGISTERED		(1<<0)	// If this is set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY	(1<<1)	// Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL			(1<<2)	// defined by the game DLL
#define FCVAR_CLIENTDLL			(1<<3)  // defined by the client DLL
#define FCVAR_HIDDEN			(1<<4)	// Hidden. Doesn't appear in find or auto complete. Like DEVELOPMENTONLY, but can't be compiled out.

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
	char* str;
	std::uint32_t clr_rgba;
	std::array<float, 2> two_floats;
	std::array<float, 3> three_floats;
	std::array<float, 4> four_floats;
};

struct CVar {
	const char* name{};
	CVar* m_pNext{};
	PAD(16);
	const char* desc{};
	EConvarType type{};
	uint32_t timesChanged{};
	uint32_t flags{};
	PAD(4);
	int m_iCallbackIndex{};
	PAD(4);
	CVarValue value{};
	CVarValue defaultValue{};

	// Sets string value to a stringified integer
	void SetStrVal(int val) {
		_itoa(val, value.str, 10);
	}
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
	std::uint64_t impl = 0xFFFFFFFF;
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

// CCVar / ICVar
// tier0.dll
class CCVar : public VClass {
public:

	static inline std::map<std::string, CVarID> CVars{};
	using CVarCallbackFn = void* (*)(const CVarID& id, int unk1, const CVarValue* val, const CVarValue* old_val);

	CVarCallbackFn& GetCallback(int id) {
		return *(CVarCallbackFn*)(Member<uintptr_t>(0x80) + 24 * id);
	}

	void TriggerCallback(const CVarID& id) {
		const auto cvar = id.m_pVar;
		GetCallback(cvar->m_iCallbackIndex)(id, 0, &cvar->value, &cvar->value);
	}

	GETTER(uint32_t, GetCVarCount, 0xA0);
	GETTER(CVarNode*, GetCVarNodeList, 0x40);

	// Removes "hidden" and "dev only" flags from convars
	void UnlockHiddenConVars() {
		auto list = GetCVarNodeList();
		for (int i = 0; i < GetCVarCount(); i++) {
			if (!list[i].var)
				continue;

			list[i].var->flags &= ~FCVAR_HIDDEN;
			list[i].var->flags &= ~FCVAR_DEVELOPMENTONLY;
		}
	}

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
