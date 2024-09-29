#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <map>
#include <span>
#include <array>
#include "../Base/Definitions.h"
#include "../Base/VClass.h"

// You will find these as strings in client.dll JS binding code
// They're used as arguments to a function, immediately after the string is a pointer to the value
enum CVarFlags : uint32_t {
	FCVAR_NONE = 0,
	FCVAR_DEVELOPMENTONLY = 2,
	FCVAR_HIDDEN = 16,
	FCVAR_PROTECTED = 32,
	FCVAR_SPONLY = 64,
	FCVAR_ARCHIVE = 0x80,
	FCVAR_NOTIFY = 0x100,
	FCVAR_USERINFO = 0x200,
	FCVAR_UNLOGGED = 0x800,
	FCVAR_REPLICATED = 0x2000,
	FCVAR_CHEAT = 0x4000,
	FCVAR_PER_USER = 0x8000,
	FCVAR_DEMO = 0x10000,
	FCVAR_DONTRECORD = 0x20000,
	FCVAR_NOT_CONNECTED = 0x400000,
	FCVAR_VCONSOLE_SET_FOCUS = 0x8000000
};

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

	// Set string value to a stringified integer
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

	GETTER(uint32_t, GetCVarCount, 0x70);
	GETTER(CVarNode*, GetCVarNodeList, 0x40);

	// Removes "hidden" and "dev only" flags from convars
	void UnlockHiddenConVars();

	void DumpConVarsToMap() const;

	CVar* FindConVar(uint32_t id) const {
		return GetVFunc(37).Call<CVar*>(id);
	}

	static CCVar* Get();
};
