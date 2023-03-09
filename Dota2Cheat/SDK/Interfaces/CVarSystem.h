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

	CVarCallbackFn GetCallback(int id);
	void TriggerCallback(CVarInfo info);
	uint32_t GetCVarCount();
	std::span<CVarNode, MAX_CVAR_COUNT> GetCVarNodeList();;

	void DumpConVarsToMap();
	void DumpConVarsToFile(const char* path);
	void SetConvars();

};
