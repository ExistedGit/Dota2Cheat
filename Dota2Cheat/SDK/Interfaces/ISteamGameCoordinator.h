#pragma once
#include <google/protobuf/message.h>
#include "../Enums.h"

// from steamworks sdk
class ISteamGameCoordinator
{
public:
	// Morphling technologies
	bool SendMsg(const google::protobuf::Message& message, int message_id) {
		constexpr int proto_size = 8; // sizeof( ProtoBufMsgHeader_t );
		void* ptr = malloc(message.ByteSizeLong() + proto_size);
		if (!ptr) return false;
		uint32_t msgtype = message_id | (1 << 31);
		memcpy(ptr, &msgtype, sizeof(uint32_t));
		reinterpret_cast<uint32_t*>(ptr)[1] = 0;
		message.SerializeToArray((void*)((uintptr_t)ptr + proto_size), message.ByteSizeLong());

		return SendMessage_(msgtype, ptr, message.ByteSizeLong() + 8) == k_EGCResultOK;
	}

	virtual EGCResults SendMessage_(uint32_t unMsgType, const void* pubData, uint32_t cubData) = 0;
	virtual bool IsMessageAvailable(uint32_t* pcubMsgSize) = 0;
	virtual EGCResults RetrieveMessage(uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize) = 0;
};