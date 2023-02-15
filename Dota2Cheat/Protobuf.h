#pragma once
#include <optional>
#include <string>
#include <google/protobuf/message.h>


namespace Util::Protobuf {

	inline  std::optional<int32_t> GetFieldTraverseInt32(google::protobuf::Message* msg, const char* name) {
		const google::protobuf::Descriptor* desc = msg->GetDescriptor();
		const google::protobuf::Reflection* refl = msg->GetReflection();

		std::optional<int32_t> ret = std::nullopt;
		if (!desc || !refl)
			return ret;

		int fieldCount = desc->field_count();
		auto& test = desc->field(1)->name();
		for (int i = 0; i < fieldCount; i++) {
			const google::protobuf::FieldDescriptor* field = desc->field(i);
			if (!field || field->is_repeated()) continue;
			std::string fieldName = msg->DebugString();
			if (!strcmp(fieldName.c_str(), name)) {
				if (!field->is_repeated()) {
					return refl->GetInt32(*msg, field);
				}
			}

			if (field->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) { // recurse
				google::protobuf::Message* mfield = refl->MutableMessage(msg, field);
				ret = GetFieldTraverseInt32(mfield, name);
			}
		}

		return ret;
	}

	inline std::optional<uint32_t> GetFieldTraverseUInt32(google::protobuf::Message* msg, const char* name) {
		const google::protobuf::Descriptor* desc = msg->GetDescriptor();
		const google::protobuf::Reflection* refl = msg->GetReflection();

		std::optional<uint32_t> ret = std::nullopt;
		if (!desc || !refl)
			return ret;

		int fieldCount = desc->field_count();
		for (int i = 0; i < fieldCount; i++) {
			const google::protobuf::FieldDescriptor* field = desc->field(i);
			if (!field || field->is_repeated()) continue;

			if (!strcmp(field->name().c_str(), name)) {
				if (!field->is_repeated()) {
					return refl->GetUInt32(*msg, field);
				}
			}

			if (field->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) { // recurse
				google::protobuf::Message* mfield = refl->MutableMessage(msg, field);
				ret = GetFieldTraverseUInt32(mfield, name);
			}
		}

		return ret;
	}
}