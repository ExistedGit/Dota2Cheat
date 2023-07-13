#include "SpeedIndicator.h"

void Modules::M_SpeedIndicator::Draw() {
	if (!Config::Indicators::Speed)
		return;

	EntityList.ForEach<CDOTABaseNPC_Hero>([this](auto wrap) { DrawIndicatorFor(wrap); });
}
