#pragma once
#include "../Entities/CHandle.h"
#include "../Interfaces/CGameEntitySystem.h"
#include <span>

// from concommand "dump_client_projectiles"
// best constructed by creating a struct in IDA
struct C_DOTA_TrackingProjectileInfo
{
	int32_t m_iHandle;
	PAD(4);
	unsigned int m_iMoveSpeed;
	Vector m_vLocation;
	CHandle<CDOTABaseNPC> m_hSource;
	CHandle<CDOTABaseNPC> m_hTarget;
	PAD(16);
	bool m_bDodgeable;
	bool m_bIsAttack;
	bool m_bIsEvaded;
	float m_flExpireTime;
};

class C_DOTA_LinearProjectileInfo : public VClass {
public:
	GETTER(Vector, GetPos, 0x38);
};
class C_DOTA_ProjectileManager : public VClass {
	PAD(0x30);
public:
	C_DOTA_TrackingProjectileInfo* m_pTrackingProjectiles[1024];
	C_DOTA_LinearProjectileInfo* m_pLinearProjectiles[1024];

	// Intended to check whether such a gamesystem even exists
	static C_DOTA_ProjectileManager** GetPtr() {
		static C_DOTA_ProjectileManager** ptr = FindReallocatingGameSystem("C_DOTA_ProjectileManager");
		return ptr;
	}

	static C_DOTA_ProjectileManager* Get() { return *GetPtr(); }
};