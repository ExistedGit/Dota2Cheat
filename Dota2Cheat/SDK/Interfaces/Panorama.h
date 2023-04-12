#pragma once
#include "../Base/VClass.h"
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
		GETTER(CUIPanel*, GetUIPanel, 0x8);
	};

	class CUIPanel : public VClass {
	public:
		GETTER(CPanel2D*, GetPanel2D, 0x8);

		bool BHasClass(const char* szClass) {
			return CallVFunc<148, bool>(szClass);
		}
		const char* GetAttributeString(const char* attr, const char* defaultValue = "N/A") {
			return CallVFunc<279, const char*>(attr, defaultValue);
		}
		const char* GetId() {
			return Member<const char*>(0x10);
		}
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
		auto GetUIRenderDevice() {
			return Member<CUIRenderDeviceSource2*>(0xB90);
		}
	};
}