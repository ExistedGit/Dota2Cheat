#pragma once
#include "../Base/VClass.h"
#include "../Base/Vector.h"
#include <span>
#include <string_view>
//#include <D3DX11.h>

namespace Panorama {

	class CTextureDx11 : public VClass {
	public:
		//auto GetShader1() {
		//	return Member<ID3D11ShaderResourceView*>(0x10);
		//}
		//auto GetShader2() {
		//	return Member<ID3D11ShaderResourceView*>(0x18);
		//}
	};
	class CSource2UITexture : public VClass {
	public:
		auto GetTexture() {
			return *Member<CTextureDx11**>(0x28);
		}
	};

	class CUIRenderDeviceSource2 : public VClass {
	public:
		bool LoadVPK(void* output, const char* path) {
			return CallVFunc<0, bool>(output, path);
		}
	};
	class CUIPanel;

	class CPanel2D : public VClass {
	public:
		Vector2D GetPositionWithinWindow() {
			Vector2D result{ 0, 0 };
			CallVFunc<44>(0, &result.x, &result.y);
			return result;
		}
		GETTER(CUIPanel*, GetUIPanel, 0x8);
	};

	class CUIPanel : public VClass {
		void _FindChildrenWithIdTraverse(const std::string_view& id, std::vector<CUIPanel*>& result) {
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
		GETTER(CUtlVector<uint16_t>, GetClasses, 0x160);
		// Returns a list of all child elements with the specified ID at any level of nesting
		[[nodiscard]]
		std::vector<CUIPanel*> FindChildrenWithIdTraverse(const std::string_view& id) {
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

		CUIPanel* FindChildWithIdTraverse(const std::string_view& id) {
			auto children = GetChildren();
			for (auto& panel : children) {
				if (panel->GetId() && panel->GetId() == id)
					return panel;

				auto child = panel->FindChildWithIdTraverse(id);

				if (child)
					return child;
			}
			return nullptr;
		}

		bool BHasClass(uint16_t unClass) {
			auto classes = GetClasses();
			for (auto& c : classes)
				if (unClass == c)
					return true;
			return false;
		}

		void ToggleClass(uint16_t unClass) {
			CallVFunc<150>(unClass);
		}

	};

	class CDOTA_UI_HeroImage : public CPanel2D {
	public:
		GETTER(const char*, GetSrc, 0x98);
	};
	class DotaHud : public CUIPanel {
	public:
		GETTER(Vector2D, GetScreenSize, 0x54);
	};

	class PanelListNode {
		void* unk0, * unk1;
	public:
		CUIPanel* uiPanel;
	private:
		void* unk2;
	};

	class CUIEngineSource2 : public VClass {

	public:
		template<uint32_t size>
		auto GetPanelList() {
			return std::span{ Member<PanelListNode*>(0xf8), size };
		}

		uint16_t MakeSymbol(const char* string) {
			uint16_t result = 0;
			return CallVFunc<121, uint16_t>(&result, string);
		}

		bool IsValidPanelPointer(CUIPanel* panel) {
			struct  IVPBuffer {
				uint32_t unk0, unk1;
			} buf;
			return CallVFunc<VTableIndexes::CUIEngineSource2::IsValidPanelPointer, IVPBuffer*>(&buf, panel)->unk0 != 0xFFFFFFFF;
		}
		FIELD(CUtlVector<void(*)()>, GetListeners, 0xD8);
	};
}