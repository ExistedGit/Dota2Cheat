#include "CVarSystem.h"

void CCVar::UnlockHiddenConVars() {
	auto list = GetCVarNodeList();
	for (int i = 0; i < GetCVarCount(); i++) {
		if (!list[i].var)
			continue;

		list[i].var->flags &= ~(FCVAR_HIDDEN | FCVAR_DEVELOPMENTONLY);
	}
}

void CCVar::DumpConVarsToMap() const {
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