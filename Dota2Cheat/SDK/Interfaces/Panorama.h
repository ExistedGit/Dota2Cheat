#pragma once
#include "../Base/VClass.h"
#include <span>
#include <string_view>


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
	class CSource2UITexture : public NormalClass {
	private:
		char pad[256];
	public:
		auto GetTexture() {
			return *Member<CTextureDx11**>(0x30);
		}
	};

	class CUIRenderDeviceSource2 : public VClass {
	public:
		bool LoadVPK(void* output, const char* path) {
			return CallVFunc<0, bool>(output, path);
		}
	};

	class CUIPanel : public VClass {
	public:
		const char* GetName() {
			return Member<const char*>(0x10);
		}
	};
	class PanelListNode {
		void* unk0, * unk1;
	public:
		CUIPanel* panel;
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