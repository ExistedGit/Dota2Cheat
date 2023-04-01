#pragma once
#include "../SDK/pch.h"

#pragma pack( push, 1 )
struct ProtoBufMsgHeader_t
{
	int32			m_EMsgFlagged;			// High bit should be set to indicate this message header type is in use.  The rest of the bits indicate message type.
	uint32			m_cubProtoBufExtHdr;	// Size of the extended header which is a serialized protobuf object.  Indicates where it ends and the serialized body protobuf begins.
};
#pragma pack(pop)

EGCResults hkISteamGameCoordinator__SendMessage(uintptr_t thisptr, uint32 unMsgType, ProtoBufMsgHeader_t* pubData, uint32 cubData) {

}