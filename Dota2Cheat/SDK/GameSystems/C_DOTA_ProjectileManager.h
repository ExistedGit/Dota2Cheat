#pragma once
#include "../Interfaces/CGameEntitySystem.h"
#include <span>

class C_DOTA_TrackingProjectileInfo : public VClass {
public:
	GETTER(int32_t, GetMoveSpeed, 0xC);
	GETTER(Vector, GetPos, 0x10);
	GETTER(float, GetExpireTime, 0x34);
	GETTER(bool, IsDodgeable, 0x30);
	GETTER(bool, IsAttack, 0x31);
	GETTER(bool, IsEvaded, 0x32);

	auto GetSource() {
		return Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(H2IDX(Member<uint32_t>(0x1c)));
	}

	auto GetTarget() {
		return Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(H2IDX(Member<uint32_t>(0x20)));
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
public:	
	auto GetTrackingProjectiles() {
		return std::span<C_DOTA_TrackingProjectileInfo*, 1024>{MemberInline<C_DOTA_TrackingProjectileInfo*>(0x38), 1024};
	}

	auto GetLinearProjectiles() {
		return std::span<C_DOTA_LinearProjectileInfo*, 1024>{MemberInline<C_DOTA_LinearProjectileInfo*>(0x2038), 1024};
	}
};