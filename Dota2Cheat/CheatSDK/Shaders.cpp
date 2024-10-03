#include "Shaders.h"
#include <Shaders/Vertex.h>

#include <Shaders/Greyscale.h>
#include <Shaders/NoMana.h>

#include "Data/DrawData.h"

void GreyscaleShaderCallback(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
	auto ctx = DrawData.Dx.pContext;
	static ID3D11PixelShader* g_pGreyscale = nullptr;
	static ID3D11InputLayout* il = nullptr;

	static D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	if (!g_pGreyscale) DrawData.Dx.pDevice->CreatePixelShader(g_PSGreyscale, sizeof(g_PSGreyscale), nullptr, &g_pGreyscale);
	if (!il) DrawData.Dx.pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), g_VSVertex, sizeof(g_VSVertex), &il);

	ctx->IASetInputLayout(il);
	ctx->PSSetShader(g_pGreyscale, nullptr, 0);
}
void NoManaShaderCallback(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
	auto ctx = DrawData.Dx.pContext;
	static ID3D11PixelShader* pShader = nullptr;
	static ID3D11InputLayout* il = nullptr;

	static D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	if (!pShader) DrawData.Dx.pDevice->CreatePixelShader(g_PSNoMana, sizeof(g_PSNoMana), nullptr, &pShader);
	if (!il) DrawData.Dx.pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), g_VSVertex, sizeof(g_VSVertex), &il);

	ctx->IASetInputLayout(il);
	ctx->PSSetShader(pShader, nullptr, 0);
}
