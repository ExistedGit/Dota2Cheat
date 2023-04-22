#pragma once
#include "../Base/VClass.h"
#include "../Base/Vector.h"
#include <span>
#include <string_view>
//#include <D3DX11.h>

namespace Panorama {
	// reversed via -dev launch option
	// F6 -> click on an element
	// Press "Developer data" or whatever it's named in English, it will show you an address for each panel
	// BHasClass is vfunc at offset 0x4a0(index 148), and GetClasses is right above it(returns CUtlVector<uint16_t> at 0x160)
	// So examine the elements at 0x160
	enum class PClass : uint16_t {
		TopBarHeroImage = 7037
	};

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
		void _FindChildrenWithClassTraverse(PClass unClass, std::vector<CUIPanel*>& result) {
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
		GETTER(CUtlVector<PClass>, GetClasses, 0x160);
		// Returns a list of all child elements with the specified ID at any level of nesting
		[[nodiscard]]
		std::vector<CUIPanel*> FindChildrenWithIdTraverse(const std::string_view& id) {
			std::vector<CUIPanel*> result;
			_FindChildrenWithIdTraverse(id, result);
			return result;
		}
		[[nodiscard]]
		std::vector<CUIPanel*> FindChildrenWithClassTraverse(PClass unClass) {
			std::vector<CUIPanel*> result;
			_FindChildrenWithClassTraverse(unClass, result);
			return result;
		}

		bool BHasClass(PClass unClass) {
			auto classes = GetClasses();
			for (auto& c : classes)
				if (unClass == c)
					return true;
			return false;
		}

		void ToggleClass(PClass unClass) {
			CallVFunc<0x4b0 / 8>(unClass);
		}

	};

	class CDOTA_UI_HeroImage : public CPanel2D {
	public:
		GETTER(const char*, GetSrc, 0x98);
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
		bool IsValidPanelPointer(CUIPanel* panel) {
			struct  IVPBuffer {
				uint32_t unk0, unk1;
			} buf;
			return CallVFunc<VTableIndexes::CUIEngineSource2::IsValidPanelPointer, IVPBuffer*>(&buf, panel)->unk0 != 0xFFFFFFFF;
		}

	};
}