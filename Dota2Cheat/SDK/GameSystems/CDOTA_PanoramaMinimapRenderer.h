#pragma once
#include "../Base/Vector.h"

class CDOTAPanoramaMinimapRenderer {
public:
	char pad_0000[16]; //0x0000
	Vector2D MinimapBounds; //0x0010
	char pad_0018[148]; //0x0018
	int32_t MinimapSizeX; //0x00AC
	int32_t MinimapSizeY; //0x00B0

	Vector2D GetMinimapSize() {
		return Vector2D((float)MinimapSizeX, (float)MinimapSizeY);
	}

	//Vector2D WorldToMap(const Vector& pos) {
	//	uintptr_t a1 = (uintptr_t)this + 0x10;
	//	Vector2D res;
	//	float scaler; // xmm2_4
	//	float maxBounds; // xmm8_4
	//	float v10; // xmm3_4
	//	float v11; // xmm4_4
	//	int v12; // er9
	//	float v13; // xmm13_4
	//	float v14; // xmm14_4
	//	float v15; // xmm9_4
	//	float v16; // xmm8_4
	//	float v17; // xmm2_4
	//	float v18; // xmm4_4
	//	float v19; // xmm3_4
	//	__int64 i; // rsi
	//	bool v21; // zf
	//	float v22; // xmm0_4
	//	float v23; // xmm0_4
	//	float v24; // xmm1_4
	//	__int64 v25; // rax
	//	float v26; // xmm4_4
	//	float v27; // xmm5_4
	//	float v28; // xmm6_4
	//	float v29; // xmm2_4
	//	float v30; // xmm3_4
	//	float v31; // xmm4_4
	//	float v32; // xmm2_4
	//	float v33; // xmm0_4
	//	float v34; // xmm1_4
	//	DWORD* v35; // rbx
	//	__int64 v36; // rax
	//	int v37; // xmm2_4
	//	int v38; // xmm1_4
	//	__int64 v39; // rcx
	//	int v40; // xmm0_4
	//	unsigned __int64 v43; // [rsp+40h] [rbp-C8h] BYREF
	//	int v44; // [rsp+48h] [rbp-C0h]
	//	char v45[16]; // [rsp+50h] [rbp-B8h] BYREF
	//	__int64 v46; // [rsp+60h] [rbp-A8h] BYREF
	//	int v47; // [rsp+68h] [rbp-A0h]
	//	int v48; // [rsp+6Ch] [rbp-9Ch]
	//	int v49; // [rsp+70h] [rbp-98h]
	//	int v50; // [rsp+74h] [rbp-94h]
	//	int v51; // [rsp+78h] [rbp-90h]
	//	int v52; // [rsp+7Ch] [rbp-8Ch]
	//	__int64 v54; // [rsp+188h] [rbp+80h] BYREF

	//	scaler = *(float*)(a1 + 12);
	//	maxBounds = *(float*)(a1 + 4);
	//	v10 = (float)(-1.0 / scaler) * (float)(pos.y - maxBounds);
	//	v11 = (float)(pos.x - *(float*)a1) / scaler;
	//	v12 = *(DWORD*)(a1 + 156);
	//	v13 = 512;
	//	v14 = 512;
	//	v15 = *(float*)a1 + (float)(scaler * v11);
	//	v16 = maxBounds - (float)(scaler * v10);
	//	v17 = *(float*)(a1 + 16) * 0.0009765625;

	//	v26 = *(float*)(a1 + 12);
	//	v27 = *(float*)a1;
	//	v28 = *(float*)(a1 + 4);
	//	v29 = -1.0 / v26;

	//	v30 = (float)((float)(v15 - v27) / v26) + (float)(v23 * 0.0625);
	//	v31 = (float)((float)(v16 - v28) * v29) - (float)(v24 * 0.0625);

	//	v32 = *(float*)(a1 + 16) * 0.0009765625;
	//	v33 = (float)(*(DWORD*)(a1 + 160) - *(DWORD*)(a1 + 176) - *(DWORD*)(a1 + 168));
	//	v34 = (float)*(int*)(a1 + 168);

	//	res.x = (((v30 - v13) * v32) + 0.5f)

	//		* (float)(*(DWORD*)(a1 + 156) - *(DWORD*)(a1 + 164) - *(DWORD*)(a1 + 172))

	//		+ (float)*(int*)(a1 + 164);
	//	res.y = (float)((float)((float)((float)(v31 - v14) * v32) + 0.5) * v33) + v34;
	//}

};

