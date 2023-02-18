#pragma once

#define PROTOBUF_NAMESPACE_OPEN namespace google { namespace protobuf {
#define PROTOBUF_NAMESPACE_CLOSE } }
#define PROTOBUF_NAMESPACE_ID google::protobuf
#define PROTOBUF_CONSTEXPR constexpr
#define PROTOBUF_ATTRIBUTE_REINITIALIZES
#define PROTOBUF_NODISCARD [[nodiscard]]
#define PROTOBUF_ASSUME(pred) GOOGLE_DCHECK(pred)