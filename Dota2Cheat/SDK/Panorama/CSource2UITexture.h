#pragma once
#include "../Base/VClass.h"
#include "../Base/Vector.h"
#include "../Base/Definitions.h"
#include <span>
#include <string_view>
#include <d3d11.h>

class CTextureDx11 : public VClass {
public:
	GETTER(ID3D11ShaderResourceView*, GetShaderSRGB, 0x10);
	GETTER(ID3D11ShaderResourceView*, GetShaderUNORM, 0x18);
};

class CSource2UITexture : public VClass {
public:
	auto GetTexture() {
		return *Member<CTextureDx11**>(0x28);
	}
};