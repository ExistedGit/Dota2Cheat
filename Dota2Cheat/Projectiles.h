#pragma once
#include "sdk.h"

class C_DOTA_TrackingProjectileInfo : public VClass {
public:
	int32_t GetMoveSpeed() {
		return Member<uint32_t>(0xC);
	}
	Vector GetPos() {
		return Member<Vector>(0x10);
	}
	uint32_t GetSource() {
		return Member<uint32_t>(0x1c);
	}
	uint32_t GetTarget() {
		return Member<uint32_t>(0x20);
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
};
class C_DOTA_ProjectileManager : public VClass {

public:
	std::span<C_DOTA_TrackingProjectileInfo*, 1024> GetTrackingProjectiles() {
		return std::span<C_DOTA_TrackingProjectileInfo*, 1024>{(C_DOTA_TrackingProjectileInfo**)((uintptr_t)this + 0x38), 1024};
	}
};