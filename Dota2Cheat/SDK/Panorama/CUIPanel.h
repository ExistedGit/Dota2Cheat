#pragma once
#include "../Base/VClass.h"
#include "../Base/CUtlVector.h"
#include "../Base/Definitions.h"
#include <vector>

class CPanel2D;

class CUIPanel : public VClass {
	void _FindChildrenWithIdTraverse(std::string_view id, std::vector<CUIPanel*>& result) {
		auto children = GetChildren();
		for (auto& panel : children) {
			if (panel->GetId() && panel->GetId() == id)
				result.push_back(panel);
			panel->_FindChildrenWithIdTraverse(id, result);
		}
	}
	void _FindChildrenWithClassTraverse(uint16_t unClass, std::vector<CUIPanel*>& result) {
		auto children = GetChildren();
		for (auto& panel : children) {
			if (panel->BHasClass(unClass))
				result.push_back(panel);
			panel->_FindChildrenWithClassTraverse(unClass, result);
		}
	}
public:
	GETTER(CPanel2D*, GetPanel2D, 0x8);
	GETTER(const char*, GetId, 0x10);
	GETTER(CUIPanel*, GetParent, 0x18);
	GETTER(CUtlVector<CUIPanel*>, GetChildren, 0x28);
	GETTER(CUtlVector<uint16_t>, GetClasses, 0x158);
	// Returns a list of all child elements with the specified ID at any level of nesting
	[[nodiscard]]
	std::vector<CUIPanel*> FindChildrenByIdTraverse(std::string_view id) {
		std::vector<CUIPanel*> result;
		_FindChildrenWithIdTraverse(id, result);
		return result;
	}


	[[nodiscard]]
	std::vector<CUIPanel*> FindChildrenWithClassTraverse(uint16_t unClass) {
		std::vector<CUIPanel*> result;
		_FindChildrenWithClassTraverse(unClass, result);
		return result;
	}

	CUIPanel* FindChildByIdTraverse(std::string_view id) {
		auto children = GetChildren();
		for (auto& panel : children) {
			if (panel->GetId() && panel->GetId() == id)
				return panel;

			auto child = panel->FindChildByIdTraverse(id);

			if (child)
				return child;
		}
		return nullptr;
	}

	void SetActive(bool state) {
		CallVFunc<31>(state);
	}

	void ApplyStyles(char unk = 1) {
		CallVFunc<70>(unk);
	}

	void RemoveClass(uint16_t class_) {
		CallVFunc<138, void>(class_);
	}

	void AddClass(const char* class_);

	void RemoveClass(const char* class_);

	bool BHasClass(uint16_t unClass) const {
		return GetVFunc(VMI::CUIPanel::BHasClass)(unClass);
	}

	void ToggleClass(uint16_t unClass) {
		CallVFunc<150>(unClass);
	}

};
class CDOTA_Hud : public CUIPanel {
public:
	GETTER(Vector2D, GetScreenSize, 0x54);
};
