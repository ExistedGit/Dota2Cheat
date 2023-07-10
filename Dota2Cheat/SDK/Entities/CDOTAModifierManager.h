#pragma once
#include "CDOTAModifier.h"
#include "../Base/CUtlVector.h"
#include <array>

struct ModifierFunctionListNode {
	struct ReturnBuffer {
		BYTE unk[0x100];
	};

	CDOTAModifier* modifier;
	using GetModifierPropertyValue = float(*)(CDOTAModifier*, void*, void*, void*);
	GetModifierPropertyValue func;

	float GetPropertyValue() {
		ReturnBuffer buf{};

		return func(modifier, &buf, &buf, nullptr);
	}
private:
	uintptr_t unk;
};

class CDOTAModifierManager : public VClass {
public:
	FIELD(CUtlVector<CDOTAModifier*>, GetModifierList, 0x10);

	// Basically how this works:
	// You have a 272-element array of uint16
	// ModifierFunction enum members correspond to indices in that array
	// matrix[modFunc] gives you the index of the CUtlVector in what GetModifierFunctionList gets 
	// (or 0xFFFF if there was no modifier with such a function)
	// In CUtlVector are all the modifiers with the specified function
	auto GetModifierFunctionMatrix() const {
		return std::span<uint16_t, 272>(MemberInline<uint16_t>(0x40), 272);
	}

	GETTER(CUtlVector<CUtlVector<ModifierFunctionListNode>>, GetModifierFunctionList, 0x28);


	CUtlVector<ModifierFunctionListNode>* GetBuffsByModifierFunction(ModifierFunction id) const {
		if (const uint16_t idx = GetModifierFunctionMatrix()[id]; idx != MODIFIER_FUNCTION_INVALID) {
			auto list = GetModifierFunctionList();
			if (idx < list.m_Size)
				return &list.at(idx);
		}
		return nullptr;
	}

	float GetModifierPropertySum(ModifierFunction id) const {
		float sum = 0;
		auto vec = GetBuffsByModifierFunction(id);
		if (!vec)
			return sum;

		for (auto& node : *vec)
			sum += node.GetPropertyValue();
		return sum;
	}
};
