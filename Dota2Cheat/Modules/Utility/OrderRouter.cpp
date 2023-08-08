#include "OrderRouter.h"

bool Modules::M_OrderRouter::RouteOrder(NetMessageHandle_t* messageHandle, google::protobuf::Message* msg) {
	if (messageHandle->messageID != 350)
		return true;

	bool execOrder = true;
	auto orderMsg = (CDOTAClientMsg_ExecuteOrders*)msg;
	auto& order = orderMsg->mutable_orders()->at(0);
	auto s = orderMsg->mutable_orders()->begin() + 1;
	orderMsg->mutable_orders()->erase(s);

	auto pos = order.mutable_position();
	Vector vectorPos{ pos->x(), pos->y(), pos->z() };
	uint32_t abilityIdx =
		order.ability_index(),
		targetIdx = order.target_index(),
		flags = order.flags(),
		seqNum = order.has_sequence_number();
	auto issuer = Interfaces::EntitySystem->GetEntity(order.mutable_units()->at(0));

	if (issuer->GetLifeState() != 0)
		return false;

	auto orderType = order.order_type();

	switch (orderType) {
	case DOTA_UNIT_ORDER_CAST_POSITION: {
		Modules::PerfectBlink.AdjustIfBlink(&vectorPos, abilityIdx, issuer);
		break;
	}
	case DOTA_UNIT_ORDER_CAST_TARGET: {
		Modules::CastRedirection.RedirectIfIllusionCast(targetIdx, issuer, abilityIdx, execOrder);
		break;
	}
	}

	if (orderType == DOTA_UNIT_ORDER_CAST_NO_TARGET ||
		orderType == DOTA_UNIT_ORDER_CAST_POSITION ||
		orderType == DOTA_UNIT_ORDER_CAST_TARGET)
		if (Modules::BadCastPrevention.IsBadCast(orderType, targetIdx, &vectorPos, abilityIdx, issuer))
			return false;
	
	order.set_ability_index(abilityIdx);
	order.set_target_index(targetIdx);

	pos->set_x(vectorPos.x);
	pos->set_y(vectorPos.y);
	pos->set_z(vectorPos.z);

	return execOrder;
}
