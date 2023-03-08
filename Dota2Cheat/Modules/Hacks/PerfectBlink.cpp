#include "PerfectBlink.h"

bool Hacks::PerfectBlink::AdjustIfBlink(Vector* position, uint32_t abilityIndex, CBaseEntity* issuer) {
	// Blink overshoot bypass
	auto item = Interfaces::EntitySystem->GetEntity<CDOTABaseAbility>(abilityIndex);

	if (!strstr(item->GetIdentity()->GetName(), "blink"))
		return false;

	auto maxDist = item->GetEffectiveCastRange();
	auto pos2D = position->To<Vector2D>();

	if (IsWithinRadius(issuer->GetPos(), *position, maxDist))
		return false;

	auto dist = issuer->GetPos().To<Vector2D>().DistTo(pos2D);
	// Relative vector from the hero to the click point
	auto vec = pos2D - issuer->GetPos().To<Vector2D>();
	// Shrinking the vector so the cast point is in the range
	// -1% to make it 100% be inside the radius
	vec.x *= maxDist / dist * 0.99f;
	vec.x += issuer->GetPos().x;
	vec.y *= maxDist / dist * 0.99f;
	vec.y += issuer->GetPos().y;

	position->x = vec.x;
	position->y = vec.y;

	return true;
}
