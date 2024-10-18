#pragma once

#include "../../Base/VClass.h"
#include "../../Base/CUtlVector.h"
#include "../../Base/Definitions.h"
#include <dota_gcmessages_msgid.pb.h>

template<typename T = google::protobuf::Message>
class CProtobufMsgBase : public VClass {
private:
	void* unk;
public:
	google::protobuf::Message* header;
	EDOTAGCMsg msgID;
	T* msg;
};

class IMsgNetPacket : public VClass {
public:
	GETTER(EDOTAGCMsg, GetEMsg, 0x78);
};

class CProtobufSharedObjectBase : public VClass {
public:
	VGETTER(google::protobuf::Message*, GetPObject, 9);

	template<typename T>
	T* GetPObj() const {
		return (T*)GetPObject();
	}
};
