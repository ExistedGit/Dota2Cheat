#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <map>
#define CVAR_MAX_INDEX 4200

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
union ConVarValue
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

struct ConVar {
	const char* name{};
	ConVar* m_Pnext{};
	uintptr_t unk1{};
	uintptr_t unk2{};
	const char* desc{};
	EConvarType type{};
	int unk_maybe_number_of_times_changed{};
	int flags{};
	int unk4{};
	int CALLBACK_INDEX{};
	int unk5{};
	ConVarValue value{};
	ConVarValue defaultValue{};
};
struct CVarNode {
	ConVar* var;
	void* unk;
};

struct ConVarID
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
class CVarSystem {
public:
	static std::map<ConVar*, int> CVar;
	//static std::map<const char*,>
	using t_CvarCallback = void(*)(const ConVarID& id, int unk1, const ConVarValue* val, const ConVarValue* old_val);


	t_CvarCallback GetCallback(int id) {
		return *(t_CvarCallback*)(*(uintptr_t*)((uintptr_t)this + 0x80) + 24 * id);
	}
	void TriggerCallback(ConVar* conVar, int idx) {
		int index = CVar[conVar];
		const auto old_val = conVar->value;
		ConVarID cvid;
		cvid.impl = static_cast<uint64_t>(idx);
		cvid.var_ptr = conVar;
		GetCallback(conVar->CALLBACK_INDEX)(cvid, 0, &conVar->value, &old_val);
	}
	CVarNode* GetCVarNodeList() {
		//return std::span<const CVarNode>{ (const CVarNode*)((uintptr_t)this + 0x40), *(uint16_t*)((uintptr_t)this + 0x5A) };
		return (CVarNode*)(*(uintptr_t*)((uintptr_t)this + 0x40));
	};
	void DumpConVarsToMap() {
		auto list = GetCVarNodeList();
		for (int i = 0; i < 4200; i++) {
			if (list[i].var != nullptr &&
				list[i].var->name != nullptr)
				CVar[list[i].var] = i;
		}
	}
	void DumpConVarsToFile(const char* path) {
		std::ofstream file{ path };
		if (file.is_open()) {
			auto list = GetCVarNodeList();
			for (int i = 0; i < 4200; i++) {
				if (list[i].var != nullptr &&
					list[i].var->name != nullptr)
					file << list[i].var->name << " " << i << '\n';
			}

			file.close();
		}
	}
	void SetConvars() {
		ConVar* dota_camera_distance,
			*r_farz = nullptr,
			*dota_use_particle_fow = nullptr,
			*sv_cheats = nullptr,
			*fog_enable = nullptr;
		{
			auto list = GetCVarNodeList();
			for (int i = 0; i < CVAR_MAX_INDEX; i++) {
				auto cvar = list[i].var;
				if (strcmp(cvar->name, "dota_camera_distance") == 0) {
					dota_camera_distance = cvar;
					dota_camera_distance->value.flt = 1800.0f;
					TriggerCallback(dota_camera_distance, i);
					
				}
				else if (strcmp(cvar->name, "r_farz") == 0)
					r_farz = cvar;
				else if (strcmp(cvar->name, "dota_use_particle_fow") == 0)
					dota_use_particle_fow = cvar;
				else if (strcmp(cvar->name, "sv_cheats") == 0)
					sv_cheats = cvar;
				else if (strcmp(cvar->name, "fog_enable") == 0)
					fog_enable = cvar;
			}
			//auto camDistNode = Interfaces::CVar->GetCVarNodeList()[3437];
			//dota_camera_distance = camDistNode.;
			//fog_enable = Interfaces::CVar->GetCVarNodeList()[17].var;
			//farZ = Interfaces::CVar->GetCVarNodeList()[3763].var;
			//fow = Interfaces::CVar->GetCVarNodeList()[3098].var;
			//svCheats = Interfaces::CVar->GetCVarNodeList()[307].var;*/
			//std::cout << std::dec << dota_camera_distance->value.flt << " " << dota_camera_distance->CALLBACK_INDEX << std::hex << '\n';
		}
		sv_cheats->value.boolean = true;
		r_farz->value.flt = 5000.0f;
		dota_use_particle_fow->value.boolean = false;
		fog_enable->value.boolean = false;
	}

};