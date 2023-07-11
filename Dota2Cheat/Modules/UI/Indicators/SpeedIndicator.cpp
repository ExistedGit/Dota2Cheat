#include "SpeedIndicator.h"

void Modules::M_SpeedIndicator::Draw() {
	if (!Config::Indicators::Speed)
		return;

	EntityList.ForEachOfType(EntityType::Hero, [this](const auto& wrap) { DrawIndicatorFor(wrap); });
}
