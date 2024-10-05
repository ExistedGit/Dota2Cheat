#include "PerfectBlink.h"
#include "../../CheatSDK/include.h"

bool Modules::M_PerfectBlink::AdjustIfBlink(Vector* position, uint32_t abilityIndex, CBaseEntity* issuer) {
	if (!Config::PerfectBlink)
		return false;

	auto item = CEntSys::Get()->GetEntity<CDOTABaseAbility>(abilityIndex);

	std::string_view itemName = item->GetIdentity()->GetName();
	if (!itemName.starts_with("item_") ||
		itemName.find("blink") == -1 )
		return false;

	auto maxDist = item->GetEffectiveCastRange();
	auto castPos2D = Vector2D(*position);

	if (IsWithinRadius(issuer->GetPos(), *position, maxDist))
		return false;


	auto dist = issuer->GetPos().As<Vector2D>().DistTo(castPos2D);
	// Relative vector from the hero to the click point
	auto vec = castPos2D - issuer->GetPos().As<Vector2D>();
	// Shrinking the vector so the cast point is in range
	// -1% to guarantee it being inside the radius
	vec.x *= maxDist / dist * 0.99f;
	vec.x += issuer->GetPos().x;
	vec.y *= maxDist / dist * 0.99f;
	vec.y += issuer->GetPos().y;

	position->x = vec.x;
	position->y = vec.y;

	return true;
}
