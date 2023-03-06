#pragma once
#include "../Interfaces/CGameEntitySystem.h"
#include <span>

class C_DOTA_TrackingProjectileInfo : public VClass {
public:
	int32_t GetMoveSpeed() {
		return Member<uint32_t>(0xC);
	}
	Vector GetPos() {
		return Member<Vector>(0x10);
	}
	auto GetSource() {
		return Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(H2IDX(Member<uint32_t>(0x1c)));
	}

	auto GetTarget() {
		return Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(H2IDX(Member<uint32_t>(0x20)));
	}

	float GetExpireTime() {
		return Member<float>(0x34);
	}

	bool IsDodgeable() {
		return Member<bool>(0x30);
	}
	bool IsAttack() {
		return Member<bool>(0x31);
	}
	bool IsEvaded() {
		return Member<bool>(0x32);
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
class C_DOTA_ProjectileManager : public VClass {

public:
	auto GetTrackingProjectiles() {
		return std::span<C_DOTA_TrackingProjectileInfo*, 1024>{MemberInline<C_DOTA_TrackingProjectileInfo*>(0x38), 1024};
	}
};