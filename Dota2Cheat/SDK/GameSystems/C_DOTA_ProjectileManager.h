#pragma once
#include "../Interfaces/CGameEntitySystem.h"
#include <span>

class C_DOTA_TrackingProjectileInfo : public VClass {
public:
	GETTER(int32_t, GetMoveSpeed, 0x4);
	GETTER(Vector, GetPos, 0x8);
	GETTER(bool, IsDodgeable, 40);
	GETTER(bool, IsAttack, 41);
	GETTER(bool, IsEvaded, 42);
	GETTER(float, GetExpireTime, 44);

	auto GetSource() {
		return Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(H2IDX(Member<uint32_t>(20)));
	}

	auto GetTarget() {
		return Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(H2IDX(Member<uint32_t>(24)));
	}

	Vector PredictPos(float deltaTime) {
		auto pos = GetPos();
		Vector predictedPos{};
		auto moveVec = GetTarget()->GetPos() - pos;
		auto moveDelta = moveVec.Length2D();
		moveVec *= GetMoveSpeed() * deltaTime / moveDelta;

		predictedPos = pos + moveVec;
		return predictedPos;
	}
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