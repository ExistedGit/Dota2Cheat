#pragma once
#include "../Entities/CHandle.h"
#include "../Interfaces/CGameEntitySystem.h"
#include <span>

struct C_DOTA_TrackingProjectileInfo
{
	int32_t m_iHandle; // 0x0
	int32_t m_iMoveSpeed; // 0x4
	Vector m_vLocation; // 0x8
	CHandle<CDOTABaseNPC> m_hSource; // 0x14
	CHandle<CDOTABaseNPC> m_hTarget; // 0x18
	Vector m_vTargetLocation; // 0x1c
	bool m_bDodgeable; // 0x28
	bool m_bIsAttack; // 0x29
	bool m_bIsEvaded; // 0x2a
	uint8_t pad_0x2b[1]; // 0x2b
	float m_flExpireTime; // 0x2c
	float m_flMaxImpactTime; // 0x30
	uint8_t pad_0x34[36]; // 0x34
};

class C_DOTA_LinearProjectileInfo : public VClass {
public:
	GETTER(Vector, GetPos, 0x38);
};
class C_DOTA_ProjectileManager : public VClass {
	char _pad[0x30];
public:
	C_DOTA_TrackingProjectileInfo* m_pTrackingProjectiles[1024];
	C_DOTA_LinearProjectileInfo* m_pLinearProjectiles[1024];
};