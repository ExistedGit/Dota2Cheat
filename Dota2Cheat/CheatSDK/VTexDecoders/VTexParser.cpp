#include "VTexParser.h"

VTexParser::ImageData VTexParser::Load(const char* filename) {
	static auto mem = CMemAlloc::Instance();
	static auto fs = Interfaces::FileSystem;
	auto file = fs->OpenFile(filename, "rb");
	auto fileSize = fs->Size(file);

	auto buffer = mem->Alloc<char>(fileSize);

	Interfaces::FileSystem->CallVFunc<79, char*>(buffer, fileSize, fileSize, file);
	char* blockPos = nullptr;

	char* header = nullptr;
	for (int i = 0; i < fileSize; i++) {
		if (*(uint32_t*)(buffer + i) == *(uint32_t*)"DATA") {
			header = buffer + *(uint32_t*)(buffer + i + 4) + i + 4;
			break;
		}
	}


	memstream s(header);
	VTexHeader h;
	s >> h;

	if (h.extra_data_count > 0)
	{
		s.seekg(h.extra_data_offset - 8); // 8 is 2 uint32s we just read

		for (auto i = 0; i < h.extra_data_count; i++)
		{
			VTexExtraData type{};
			auto offset = 0, size = 0;
			s >> type >> offset >> size;
			offset -= 8;


			auto prevOffset = s.pos;

			s.pos += offset;
			auto shit = new char[size];
			s.read(shit, size);
		}
	}

	auto d = s.pos + s.data;
	auto dataSize = fileSize - s.pos - (header - buffer);

	ImageData ret;
	ret.w = h.width;
	ret.h = h.height;

	using enum VTexFormat;
	switch (h.format) {
	case RGBA8:
	case BGRA8:
	{
		if (h.format == VTexFormat::BGRA8)
			for (int i = 0; i < dataSize; i += 4) {
				std::swap(d[i], d[i + 2]);
			}

		ret.data = mem->Alloc<uint8_t>(dataSize);
		memcpy(ret.data, d, dataSize);
		break;
	}
	case PNG_RGBA8: {
		int x, y, comp;
		auto png = stbi_load_from_memory((stbi_uc*)d, dataSize, &x, &y, &comp, 4);

		ret.data = mem->Alloc<uint8_t>(x * y * comp);
		memcpy(ret.data, png, x * y * comp);
		stbi_image_free(png);
		break;
	}
	case DXT5:
	{
		auto out = mem->Alloc<unsigned char>(h.width * h.height * 4);
		DXT5Decoder((uint8_t*)d, h.width, h.height).Decode(out);
		// BGRA shenanigans... again?
		for (int i = 0; i < h.width * h.height * 4; i += 4) {
			std::swap(out[i], out[i + 2]);
		}
		ret.data = out;
	}
	}

	mem->Free(buffer);
	return ret;
}
