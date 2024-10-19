#pragma once

#include "bf_read.h"
#include "bf_write.h"
#include "../../Base/Color.h"
#include "../../Base/VClass.h"
#include "../../Base/Definitions.h"
#include "../../VMI.h"

#include <array>


#include <google/protobuf/message.h>
#include <google/protobuf/io/coded_stream.h>
#include <dota_gcmessages_msgid.pb.h>
#include <econ_gcmessages.pb.h>


#define CASE_STRING( x ) case static_cast<int>( x ) : return #x

struct NetMessageInfo_t;

enum NetChannelBufType_t : int
{
    BUF_DEFAULT = -1,
    BUF_UNRELIABLE = 0,
    BUF_RELIABLE = 1,
    BUF_VOICE = 2,
};

//enum class SignonState_t : uint32_t {
//    SIGNONSTATE_NONE = 0,
//    SIGNONSTATE_CHALLENGE = 1,
//    SIGNONSTATE_CONNECTED = 2,
//    SIGNONSTATE_NEW = 3,
//    SIGNONSTATE_PRESPAWN = 4,
//    SIGNONSTATE_SPAWN = 5,
//    SIGNONSTATE_FULL = 6,
//    SIGNONSTATE_CHANGELEVEL = 7,
//};

enum NetMessageGroups : int
{
    SYSTEM = 0,
    ENTITIES = 1,
    STRING_COMMAND = 2,
    SIGNON = 3,
    SPAWNGROUPS = 4,
    MOVE = 5,
    VOICE = 6,
    GENERIC = 7,
    STRING_TABLE = 8,
    SOUNDS = 9,

    EVENTS = 12,
    CLIENT_MESSAGES = 13,
    USER_MESSAGES = 14,
    DECALS = 15,
};

enum NetworkSerializationMode_t : int
{
    NET_SERIALIZATION_MODE_0  = 0,
    NET_SERIALIZATION_MODE_DEFAULT  = 0,
    NET_SERIALIZATION_MODE_SERVER  = 0,
    NET_SERIALIZATION_MODE_1  = 1,
    NET_SERIALIZATION_MODE_CLIENT  = 1,
    NET_SERIALIZATION_MODE_COUNT  = 2,
};


class CNetPacket
{
public:
    int notsure;
    int cubData;
    void *pubData;
};


class CMsgProtoBufHeader;
class IProtoBufSendHandler
{
public:
    virtual bool BAsyncSend( EDOTAGCMsg eMsg, const uint8_t *pubMsgBytes, uint32 cubSize ) = 0;
};

class CProtoBufMsgBase
{
public:
    virtual ~CProtoBufMsgBase() = 0;
    virtual void DESTROY2() = 0;
    virtual google::protobuf::Message *GetGenericBody(void) = 0;

    void *pNetPacket;
    CMsgProtoBufHeader *pProtoBufHdr;
    EDOTAGCMsg eMsg;
    char _pad[4];
};

template<typename T>
class CProtoBufMsg : CProtoBufMsgBase
{
public:

    T *pProtoBufBody;
};

class CProtobuffBinding // AKA CNetMessagePB
{
public:
    virtual const char* GetName(void) = 0;
    virtual int GetSize(void) = 0;
    virtual const char* ToString(google::protobuf::Message *msg, void *storage) = 0;
    virtual const char *GetGroup(void) = 0;
    virtual Color GetGroupColor(void) = 0;
    virtual NetChannelBufType_t GetBufType(void) = 0;
    virtual bool ReadFromBuffer(google::protobuf::Message *msg, bf_read &) = 0; // true if parsing O = 0K
    virtual bool WriteToBuffer(google::protobuf::Message *msg, bf_write &) = 0; // true if parsing O = 0K
    virtual void* AllocateMessage(void) = 0;
    virtual void DeallocateMessage(void *) = 0;
    virtual void* AllocateAndCopyConstructNetMessage(void const* otherProtobufMsg) = 0;
    virtual bool OkToRedispatch(void) = 0;
    virtual void Copy(void const*,void *) = 0;
};

class CNetworkSerializerPB // PB must = protobuf
{
public:
    virtual void DESTROY1() = 0;
    virtual void DESTROY2() = 0;
    virtual const char* GetUnscopedName(void) = 0;
    virtual void* GetNetMessageInfo(void) = 0;
    virtual void SetMessageID(unsigned int) = 0;
    virtual void AddCategoryMask(unsigned int bitflag, bool applyToAnotherMember) = 0;
    virtual void SwitchMode(int networkValidationMode_t) = 0;
    virtual void AllocateMessage(void) = 0;
    virtual void DeallocateMessage(void *unk) = 0;
    virtual void AllocateAndCopyConstructNetMessage(void const *) = 0;
    virtual void Serialize(bf_write &, void const *, NetworkSerializationMode_t) = 0;
    virtual void UnSerialize(bf_read &, void *, NetworkSerializationMode_t) = 0;

    const char *unscopedName;
    uint32_t categoryMask;
    int unk;
    CProtobuffBinding *protobufBinding;
    const char *groupName;
    int16_t messageID;
    uint8_t groupID;
    uint8_t defaultBufferType;
    char _pad[28];
};

typedef CNetworkSerializerPB GameEventHandle_t;
typedef CNetworkSerializerPB NetMessageHandle_t;

struct MessageEntryWrapper {
    void *unk[2];
    CNetworkSerializerPB *entry;
};

struct MessageEntries {
    std::array<MessageEntryWrapper, 334> messages;
};

struct NetMessageInfo_t
{
    void* pad;
    CProtobuffBinding* pProtobufBinding;
};

class CNetworkMessages : VClass
{
public:
    static CNetworkMessages* Get();

	auto GetNetMessageInfo( NetMessageHandle_t* hnd ) {
		return CallVFunc<11, NetMessageInfo_t*>(hnd);
	}

	auto FindNetworkMessage( const char* msg_name ) {
		return CallVFunc<13, NetMessageHandle_t*>(msg_name);
	}

	auto FindNetworkMessageByID( int id ) {
		return GetVFunc(VMI::CNetworkMessages::FindNetworkMessageByID).Call<CNetworkSerializerPB*>(id);
	}

	struct CallbackData {
		uint32_t unk0;
		uint32_t unk3;
		const char* m_szCallbackName;
	private:
		uintptr_t unk1;
	public:
		void(*m_CallbackFn)(void*);
		const char* m_szClassName;
	private:
		uintptr_t unk2[2];
	};

	auto GetNetvarCallbacks() {
		return **(std::array<CallbackData, 256>**)((uintptr_t)this + 0x4B8);
	}

	Function FindCallback(std::string_view name) {
		for (auto& cb : GetNetvarCallbacks())
			if (cb.unk0 != 0x80000000 && cb.m_szCallbackName == name)
				return cb.m_CallbackFn;
		
		return nullptr;
	}

	char _pad[0x1A0 - sizeof( void* )];
	MessageEntries* messageList; // usermessage list @ +0x1F0
	// @ +0x420 some sort of protobuf descriptions
};