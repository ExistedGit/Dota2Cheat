#pragma once
#include <cstdint>

enum class VTexFlags : uint16_t {
	SUGGEST_CLAMPS = 1 << 0,
	SUGGEST_CLAMPT = 1 << 1,
	SUGGEST_CLAMPU = 1 << 2,
	NO_LOD = 1 << 3,
	CUBE_TEXTURE = 1 << 4,
	VOLUME_TEXTURE = 1 << 5,
	TEXTURE_ARRAY = 1 << 6
};

enum class VTexFormat : uint8_t {
	UNKNOWN = 0,
	DXT1 = 1,
	DXT5 = 2,
	I8 = 3,
	RGBA8 = 4,
	R16 = 5,
	RG16 = 6,
	RGBA16 = 7,
	R16F = 8,
	RG16F = 9,
	RGBA16F = 10,
	R32F = 11,
	RG32F = 12,
	RGB32F = 13,
	RGBA32F = 14,
	JPEG_RGBA8 = 15,
	PNG_RGBA8 = 16,
	JPEG_DXT5 = 17,
	PNG_DXT5 = 18,
	BC6H = 19,
	BC7 = 20,
	ATI2N = 21,
	IA8 = 22,
	ETC2 = 23,
	ETC2_EAC = 24,
	R11_EAC = 25,
	RG11_EAC = 26,
	ATI1N = 27,
	BGRA8 = 28
};

enum class VTexExtraData : uint32_t {
	UNKNOWN = 0,
	FALLBACK_BITS = 1,
	SHEET = 2,
	METADATA = 3,
	COMPRESSED_MIP_SIZE = 4,
	CUBEMAP_RADIANCE_SH = 5,
};

#pragma pack(push, 1)
struct VTexHeader {
	uint16_t version;
	VTexFlags flags;
	float reflectivity[4];
	uint16_t width;
	uint16_t height;
	uint16_t depth;
	VTexFormat format;
	uint8_t num_mip_levels;
	uint32_t picmip0_res;
	uint32_t extra_data_offset;
	uint32_t extra_data_count;
};
#pragma pack(pop)