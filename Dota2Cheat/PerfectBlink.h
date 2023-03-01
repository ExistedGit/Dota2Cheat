#pragma once
#include "Wrappers.h"
#include "Interfaces.h"

namespace Hacks {

	class PerfectBlink {
	public:
		bool AdjustIfBlink(Vector3*& position, uint32_t abilityIndex, BaseEntity* issuer) {
			// Blink overshoot bypass
			auto item = Interfaces::EntitySystem->GetEntity<BaseAbility>(abilityIndex);
			
			if (!strstr(item->GetIdentity()->GetName(), "blink"))
				return false;
			
			auto maxDist = item->GetEffectiveCastRange();
			auto pos2D = position->AsVec2();
			
			if (IsWithinRadius(issuer->GetPos2D(), pos2D, maxDist))
				return false;

			auto dist = issuer->GetPos2D().DistanceTo(pos2D);
			// Relative vector from the hero to the click point
			auto vec = pos2D - issuer->GetPos2D();
			// Shrinking the vector so the cast point is in the range
			// -1% to make it 100% be inside the radius
			vec.x *= maxDist / dist * 0.99f;
			vec.x += issuer->GetPos2D().x;
			vec.y *= maxDist / dist * 0.99f;
			vec.y += issuer->GetPos2D().y;

			position->x = vec.x;
			position->y = vec.y;

			return true;
		}
	};
}
namespace Modules {
	Hacks::PerfectBlink PerfectBlink{};
}