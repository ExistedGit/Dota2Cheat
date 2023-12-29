#include "VTexParser.h"

VTexParser::ImageData VTexParser::Load(std::string_view filename) {
	static auto mem = CMemAlloc::Instance();
	static auto fs = Interfaces::FileSystem;
	auto file = fs->OpenFile(filename.data(), "rb");
	auto fileSize = fs->Size(file);

	auto buffer = mem->Alloc<char>(fileSize);

	fs->Read(buffer, fileSize, fileSize, file);
	fs->Close(file);

	char* blockPos = nullptr;

	int hdrPos = 0;
	for (int i = 0; i < fileSize; i++) {
		if (*(uint32_t*)(buffer + i) == *(uint32_t*)"DATA") {
			hdrPos = *(uint32_t*)(buffer + i + 4) + i + 4;
			break;
		}
	}

	memstream s(buffer);
	s.pos = hdrPos;

	VTexHeader hdr;
	s >> hdr;

	auto ActualWidth = hdr.width,
		ActualHeight = hdr.height;

	int extraDataSize = 0;
	if (hdr.extra_data_count > 0)
	{
		s.seekg(hdr.extra_data_offset - 8); // 8 is 2 uint32s we just read

		for (auto i = 0; i < hdr.extra_data_count; i++)
		{
			VTexExtraData type{};
			auto offset = 0, size = 0;
			s >> type >> offset >> size;
			offset -= 8;

			extraDataSize += size;

			auto prevOffset = s.pos;

			s.pos += offset;

			if (type == VTexExtraData::METADATA)
			{
				s.pos += 2;
				uint16_t nw, nh;
				s >> nw >> nh;
				if (nw > 0 && nh > 0 && ActualWidth >= nw && ActualHeight >= nh)
				{
					ActualWidth = nw;
					ActualHeight = nh;
				}
			}

			s.pos = prevOffset;
		}
	}
	s.pos += extraDataSize;

	auto inputData = s.pos + s.data;
	auto dataSize = fileSize - s.pos;

	ImageData ret;
	ret.w = ActualWidth;
	ret.h = ActualHeight;

	using enum VTexFormat;
	switch (hdr.format) {
	case RGBA8:
	case BGRA8:
	{
		if (hdr.format == VTexFormat::BGRA8)
			for (int i = 0; i < dataSize; i += 4) {
				std::swap(inputData[i], inputData[i + 2]);
			}

		ret.data = mem->Alloc<uint8_t>(dataSize);
		memcpy(ret.data, inputData, dataSize);
		break;
	}
	case PNG_RGBA8: {
		int x, y, comp;
		auto png = stbi_load_from_memory((stbi_uc*)inputData, dataSize, &x, &y, &comp, 4);

		ret.data = mem->Alloc<uint8_t>(x * y * comp);
		memcpy(ret.data, png, x * y * comp);
		stbi_image_free(png);
		break;
	}
	case DXT5:
	{
		auto outSize = ActualWidth * ActualHeight * 4;
		auto out = mem->Alloc<unsigned char>(outSize);

		DXT5Decoder((uint8_t*)inputData, ActualWidth, ActualHeight, hdr.width, hdr.height).Decode(out);
		// BGRA shenanigans... again?
		for (int i = 0; i < outSize; i += 4) {
			std::swap(out[i], out[i + 2]);
		}
		ret.data = out;
	}
	}

	mem->Free(buffer);
	return ret;
}
