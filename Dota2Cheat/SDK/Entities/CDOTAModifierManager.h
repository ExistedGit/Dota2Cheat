#pragma once
#include "CDOTAModifier.h"
#include "../Base/CUtlVector.h"

class CDOTAModifierManager : public VClass {
public:
	// Returns the original CUtlVector that stores the list
	auto GetModifierListRaw() {
		return MemberInline<CUtlVector<CDOTAModifier*>>(0x10);
	}
	auto GetModifierList() {
		auto vecModifiers = MemberInline<CUtlVector<CDOTAModifier*>>(0x10);
		return vecModifiers->AsStdVector();
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CDOTAModifierManager>("CDOTAModifierManager");
		type["GetModifierList"] = &CDOTAModifierManager::GetModifierList;
	}
};
