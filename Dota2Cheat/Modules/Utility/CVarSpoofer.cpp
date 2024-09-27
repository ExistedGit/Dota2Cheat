#include "CVarSpoofer.h"

void Modules::M_CVarSpoofer::SpoofVar(const char* varName) {
	auto origVar = CCVar::Get()->CVars[varName].m_pVar;
	if (spoofedVars.contains(origVar))
		return;

	CVarInitInfo info;

	info.m_pDefVal = origVar->defaultValue.i64;

	auto dummyVar = CMemAlloc::Instance()->AllocInit<CVar>();

	CVarID dummyId;
	dummyId.m_pVar = dummyVar;

	auto dummyName = CMemAlloc::Instance()->Alloc<char>(strlen(origVar->name) + 1 + 4);
	sprintf(dummyName, "d2c_%s", origVar->name);

	Memory::Copy(dummyVar, origVar);
	origVar->name = dummyName;

	RegisterConVarInfo regInfo{ varName, dummyVar->desc, dummyVar->flags, info };
	CCVar::Get()->CallVFunc<35>(&regInfo, 8, &dummyId, (void*)((uintptr_t)&dummyId + 8));

	spoofedVars[dummyVar] = {
		.original = origVar,
		.impl = dummyId.impl,
	};
}

void Modules::M_CVarSpoofer::RestoreVars() {
	for (auto& [cvar, data] : spoofedVars) {
		data.original->name = cvar->name;
		CCVar::Get()->CallVFunc<38>(data.impl);
	}
}
