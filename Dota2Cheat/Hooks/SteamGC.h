#pragma once
#include "../SDK/pch.h"


namespace Hooks {
#pragma pack( push, 1 )
	struct ProtoBufMsgHeader_t
	{
		int32			m_EMsgFlagged;			// High bit should be set to indicate this message header type is in use.  The rest of the bits indicate message type.
		uint32			m_cubProtoBufExtHdr;	// Size of the extended header which is a serialized protobuf object.  Indicates where it ends and the serialized body protobuf begins.
	};
#pragma pack(pop)


	using ISteamGCSendMessageFn = EGCResults(__fastcall*)(uintptr_t thisptr, uint32 unMsgType, ProtoBufMsgHeader_t* pubData, uint32 cubData);
	inline ISteamGCSendMessageFn oISteamGCSendMessage;

	// ISteamGameCoordinator::SendMessage and ::RetrieveMessage

	inline EGCResults hkISteamGCSendMessage(uintptr_t thisptr, uint32 unMsgType, ProtoBufMsgHeader_t* pubData, uint32 cubData) {
		return oISteamGCSendMessage(thisptr, unMsgType, pubData, cubData);
	}
	using ISteamGCRetrieveMessageFn = EGCResults(__fastcall*)(uintptr_t thisptr, uint32* punMsgType, ProtoBufMsgHeader_t* pubDest, uint32 cubDest, uint32* pcubMsgSize);
	inline ISteamGCRetrieveMessageFn oISteamGCRetrieveMessage;
	inline EGCResults hkISteamGCRetrieveMessage(uintptr_t thisptr, uint32* punMsgType, ProtoBufMsgHeader_t* pubDest, uint32 cubDest, uint32* pcubMsgSize) {	
		return oISteamGCRetrieveMessage(thisptr, punMsgType, pubDest, cubDest, pcubMsgSize);
	}

	bool HookSteamGC();

}