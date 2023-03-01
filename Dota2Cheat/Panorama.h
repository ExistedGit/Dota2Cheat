#pragma once
#include "sdk.h"


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

	class CUIEngineSource2 : public VClass {

	public:
		auto GetUIRenderDevice() {
			return Member<CUIRenderDeviceSource2*>(0xB90);
		}
	};
}