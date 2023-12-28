#include "ClaimTutorialRewards.h"

void Modules::M_ClaimTutorialRewards::ClaimEventAction(uint32_t event_id, uint32_t action_id, uint32_t quantity) {
	CMsgDOTAClaimEventAction msg;
	msg.set_event_id(event_id);
	msg.set_action_id(action_id);
	msg.set_quantity(quantity);
	Interfaces::SteamGC->SendMsg(msg, k_EMsgDOTAClaimEventAction);
}

void Modules::M_ClaimTutorialRewards::ExecuteGCRequests() {
	std::vector<ClaimEventData> msgToSend;
	// 101 - 104
	for (int i = 201; i <= 204; i++)
		msgToSend.push_back({ 26, i });
	// 201 - 204
	for (int i = 201; i <= 204; i++)
		msgToSend.push_back({ 26, i });

	// 40001-40051 and 40111-40141 in increments of 10
	for (int i = 40001; i <= 40051; i += 10)
		msgToSend.push_back({ 19, i });

	for (int i = 40111; i <= 40141; i += 10)
		msgToSend.push_back({ 19, i });

	msgToSend.push_back({ 19, 40231 });

	for (auto& m : msgToSend)
		ClaimEventAction(m.first, m.second);
	msgToSend.clear();

	std::this_thread::sleep_for(std::chrono::milliseconds(1500));

	for (int i = 1001; i <= 1006; i++)
		msgToSend.push_back({ 26, i });
	for (int i = 2001; i <= 2007; i++)
		msgToSend.push_back({ 26, i });

	// 3001-3006 excluding 3005
	for (int i = 3001; i <= 3006; i++)
		if (i != 3005)
			msgToSend.push_back({ 26, i });

	for (int i = 4001; i <= 4007; i++)\
		msgToSend.push_back({ 26, i });

	for (int i = 40000; i <= 40050; i += 10)
		msgToSend.push_back({ 19, i });
	for (int i = 40110; i <= 40140; i += 10)
		msgToSend.push_back({ 19, i });

	msgToSend.push_back({ 19, 40230 });

	for (auto& m : msgToSend)
		ClaimEventAction(m.first, m.second);
	msgToSend.clear();
}
