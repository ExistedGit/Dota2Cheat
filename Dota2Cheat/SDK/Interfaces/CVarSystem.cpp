#include "CVarSystem.h"

CVarSystem::CVarCallbackFn CVarSystem::GetCallback(int id) {
	return *(CVarCallbackFn*)(Member<uintptr_t>(0x80) + 24 * id);
}

void CVarSystem::TriggerCallback(CVarInfo info) {
	const auto old_val = info.var->value;
	CVarID cvid;
	cvid.impl = static_cast<uint64_t>(info.index);
	cvid.var_ptr = info.var;
	GetCallback(info.var->CALLBACK_INDEX)(cvid, 0, &info.var->value, &old_val);
}

uint32_t CVarSystem::GetCVarCount() {
	return Member<uint32_t>(0x58);
}

std::span<CVarNode, MAX_CVAR_COUNT> CVarSystem::GetCVarNodeList() {
	return std::span<CVarNode, MAX_CVAR_COUNT>{ Member<CVarNode*>(0x40), MAX_CVAR_COUNT };
}

void CVarSystem::DumpConVarsToMap() {
	auto list = GetCVarNodeList();
	for (int i = 0; i < list.size(); i++) {
		auto var = list[i].var;
		if (var &&
			var->name) {
			CVarInfo info;
			info.var = list[i].var;
			info.index = i;

			CVar[var->name] = info;
		}
	}
}

void CVarSystem::DumpConVarsToFile(const char* path) {
	std::ofstream file{ path };
	if (file.is_open()) {
		auto list = GetCVarNodeList();
		for (int i = 0; i < list.size(); i++) {
			if (list[i].var &&
				list[i].var->name)
				file << list[i].var->name << " " << i << '\n';
		}

		file.close();
	}
}

void CVarSystem::SetConvars() {
	CVar["sv_cheats"].var->value.boolean = true;
	CVar["r_farz"].var->value.flt = 10000.0f;
	CVar["fog_enable"].var->value.boolean = false;
}
