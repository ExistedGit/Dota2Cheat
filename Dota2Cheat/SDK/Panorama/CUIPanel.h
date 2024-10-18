#pragma once
#include "../Base/VClass.h"
#include "../Base/CUtlVector.h"
#include "../Base/Definitions.h"
#include <vector>

class CPanel2D;

class CUIPanel : public VClass {
public:
	GETTER(CPanel2D*, GetPanel2D, 0x8);
	GETTER(const char*, GetId, 0x10);
	GETTER(CUIPanel*, GetParent, 0x18);
	GETTER(CUtlVector<CUIPanel*>, GetChildren, 0x28);
	GETTER(CUtlVector<uint16_t>, GetClasses, 0x158);

	// Returns a list of all child elements with the specified ID at any level of nesting
	void FindChildrenByIdTraverse(std::string_view id, std::vector<CUIPanel*>& out) const {
		auto children = GetChildren();
		for (auto& panel : children) {
			if (panel->GetId() && panel->GetId() == id)
				out.push_back(panel);
			panel->FindChildrenByIdTraverse(id, out);

		}
	}

	void FindChildrenWithClassTraverse(uint16_t unClass, std::vector<CUIPanel*>& out) const {
		auto children = GetChildren();
		for (auto& panel : children) {
			if (panel->BHasClass(unClass))
				out.push_back(panel);
			panel->FindChildrenWithClassTraverse(unClass, out);
		}
	}

	CUIPanel* FindChildByIdTraverse(std::string_view id)  const {
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

	CUIPanel* FindChildById(std::string_view id)  const {
		auto children = GetChildren();
		for (auto& panel : children) {
			if (panel->GetId() && panel->GetId() == id)
				return panel;
		}
		return nullptr;
	}

	CUIPanel* FindChildWithClassTraverse(uint16_t unClass, std::vector<CUIPanel*>& out) const {
		auto children = GetChildren();
		for (auto& panel : children) {
			if (panel->BHasClass(unClass))
				return panel;
		}
		return nullptr;
	}
	void SetActive(bool state) {
		CallVFunc<31>(state);
	}

	void ApplyStyles(char unk = 1) {
		CallVFunc<72>(unk);
	}

	void RemoveClass(uint16_t class_) {
		CallVFunc<140, void>(class_);
	}

	void AddClass(const char* class_);

	void RemoveClass(const char* class_);

	bool BHasClass(uint16_t unClass) const {
		for (auto cl : GetClasses())
			if (cl == unClass)
				return true;

		return false;
		//return GetVFunc(VMI::CUIPanel::BHasClass)(unClass);
	}

	void ToggleClass(uint16_t unClass) {
		CallVFunc<150>(unClass);
	}

};
class CDOTA_Hud : public CUIPanel {
public:
	GETTER(Vector2D, GetScreenSize, 0x54);
};
