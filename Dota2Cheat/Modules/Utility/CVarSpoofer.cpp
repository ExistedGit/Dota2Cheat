#include "CVarSpoofer.h"

void Modules::M_CVarSpoofer::SpoofVar(const char* varName) {
	auto var = Interfaces::CVar->CVars[varName].m_pVar;
	if (originalVars.contains(var))
		return;

	static Function RegisterConVar = Memory::Scan("E8 ? ? ? ? EB 4E 83 FB FF", "client.dll").GetAbsoluteAddress(1);

	auto info = CMemAlloc::Instance()->AllocInit<CVarRegInfo>();

	info->m_pDefVal = var->defaultValue.i64;

	auto mem = CMemAlloc::Instance()->AllocInit<CVarID>();
	auto InitConvar = [var](CVarID* s) {
		auto undefinedCvar = CMemAlloc::Instance()->AllocInit<CVar>();
		s->impl = 0xFFFF;
		undefinedCvar->flags = 0x400;
		undefinedCvar->type = var->type;
		s->m_pVar = undefinedCvar;
	};
	auto dummyName = CMemAlloc::Instance()->Alloc<char>(strlen(var->name) + 1 + 4);
	sprintf(dummyName, "d2c_%s", var->name);

	InitConvar(mem);
	RegisterConVar(mem, dummyName, FCVAR_CHEAT, nullptr, info);
	Memory::Copy(mem->m_pVar, var);
	var->name = dummyName;

	originalVars[mem->m_pVar] = var;
}
