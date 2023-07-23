#include "CVarSpoofer.h"

void Modules::M_CVarSpoofer::SpoofVar(const char* varName) {
	auto var = Interfaces::CVar->CVars[varName].m_pVar;
	if (originalVars.contains(var))
		return;

	auto info = CMemAlloc::Instance()->AllocInit<CVarInitInfo>();

	info->m_pDefVal = var->defaultValue.i64;

	auto mem = CMemAlloc::Instance()->AllocInit<CVarID>();
	auto InitConvar = [var](CVarID* s) {
		auto undefinedCvar = CMemAlloc::Instance()->AllocInit<CVar>();
		s->impl = 0xFFFF;
		undefinedCvar->flags = FCVAR_CHEAT;
		undefinedCvar->type = var->type;
		s->m_pVar = undefinedCvar;
	};

	auto dummyName = CMemAlloc::Instance()->Alloc<char>(strlen(var->name) + 1 + 4);
	sprintf(dummyName, "d2c_%s", var->name);

	InitConvar(mem);

	Memory::Copy(mem->m_pVar, var);
	var->name = dummyName;

	CVarID2 id{};
	id.m_pVar = mem->m_pVar;

	RegisterConVarInfo regInfo{ varName, nullptr, FCVAR_CHEAT, *info, &id, (void*)((uintptr_t)&id + 8) };
	Interfaces::CVar->CallVFunc<35>(&regInfo, 8);

	originalVars[mem->m_pVar] = var;
}
