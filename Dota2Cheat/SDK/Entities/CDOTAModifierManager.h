#pragma once
#include "CDOTAModifier.h"
#include "../Base/CUtlVector.h"

struct ModifierFunctionListNode {
	CDOTAModifier* modifier;
	void* func;
private:
	uintptr_t unk;
};

class CDOTAModifierManager : public VClass {
public:
	// Returns the original CUtlVector that stores the list
	auto GetModifierListRaw() {
		return MemberInline<CUtlVector<CDOTAModifier*>>(0x10);
	}
	auto GetModifierList() {
		auto vecModifiers = GetModifierListRaw();
		return vecModifiers->AsStdVector();
	}

	// Basically how this works:
	// You have a 256-element array of uint16
	// ModifierFunction enum members correspond to indices in that array
	// matrix[modFunc] gives you the index of the CUtlVector in what GetModifierFunctionList gets 
	// (or 0xFFFF if there was no modifier with such a function)
	// In CUtlVector are all the modifiers with the specified function
	std::span<uint16_t, 256> GetModifierFunctionMatrix() {
		return std::span<uint16_t, 256>(MemberInline<uint16_t>(0x40), 256);
	}

	auto GetModifierFunctionList() {
		return Member<CUtlVector<CUtlVector<ModifierFunctionListNode>>>(0x28);
	}

	CUtlVector<ModifierFunctionListNode>* GetBuffsByModifierFunction(ModifierFunction id) {
		if (const uint16_t idx = GetModifierFunctionMatrix()[id]; idx != MODIFIER_FUNCTION_INVALID) {
			auto list = GetModifierFunctionList();
			if (idx < list.m_Size)
				return &list.at(idx);
		}
		return nullptr;
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CDOTAModifierManager>("CDOTAModifierManager");
		type["GetModifierList"] = &CDOTAModifierManager::GetModifierList;
	}
};
