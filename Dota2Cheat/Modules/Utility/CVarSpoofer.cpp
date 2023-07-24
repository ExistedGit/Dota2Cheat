#include "CVarSpoofer.h"

void Modules::M_CVarSpoofer::SpoofVar(const char* varName) {
	auto origVar = Interfaces::CVar->CVars[varName].m_pVar;
	if (spoofedVars.contains(origVar))
		return;

	auto info = CMemAlloc::Instance()->AllocInit<CVarInitInfo>();

	info->m_pDefVal = origVar->defaultValue.i64;

	auto dummyVar = CMemAlloc::Instance()->AllocInit<CVar>();
	dummyVar->flags = FCVAR_CHEAT;
	dummyVar->type = origVar->type;

	auto dummyName = CMemAlloc::Instance()->Alloc<char>(strlen(origVar->name) + 1 + 4);
	sprintf(dummyName, "d2c_%s", origVar->name);

	Memory::Copy(dummyVar, origVar);
	origVar->name = dummyName;

	CVarID2 id{};
	id.m_pVar = dummyVar;

	RegisterConVarInfo regInfo{ varName, nullptr, FCVAR_CHEAT, *info, &id, (void*)((uintptr_t)&id + 8) };
	Interfaces::CVar->CallVFunc<35>(&regInfo, 8);

	spoofedVars[dummyVar] = {
		.original = origVar,
		.impl = regInfo.extendedId->impl,
	};
}

void Modules::M_CVarSpoofer::RestoreVars() {
	//for (auto& [cvar, data] : spoofedVars) {
	//	data.original->name = cvar->name;
	//	Interfaces::CVar->CallVFunc<38>(data.impl);
	//}
}
