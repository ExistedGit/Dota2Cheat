#pragma once
#include <cstddef>
#include <cstdint>

class DXT5Decoder {
public:
	using byte = unsigned char;

	unsigned char* input{};
	int w{}, h{},
		blockWidth{}, blockHeight{};
	int outSize = 0;

	DXT5Decoder(unsigned char* data, int width, int height, int blockWidth, int blockHeight) : input(data), w(width), h(height),
		blockWidth(blockWidth), blockHeight(blockHeight) {
		outSize = w * h * 4;
	}

	static byte ClampColor(int a)
	{
		if (a > 255)
			return 255;

		return a < 0 ? (byte)0 : (byte)a;
	}

	static void ConvertRgb565ToRgb888(uint16_t color, uint8_t& r, uint8_t& g, uint8_t& b)
	{
		int temp;

		temp = ((color >> 11) * 255) + 16;
		r = (uint8_t)(((temp / 32) + temp) / 32);
		temp = (((color & 0x07E0) >> 5) * 255) + 32;
		g = (uint8_t)(((temp / 64) + temp) / 64);
		temp = ((color & 0x001F) * 255) + 16;
		b = (uint8_t)(((temp / 32) + temp) / 32);
	}


	void DecompressBlockDXT1(int x, int y, int width, uint8_t blockStorage[8], byte* pixels, int stride);

	void Decompress8BitBlock(int bx, int w, int offset, uint64_t block, byte* pixels, int stride);

	void Decode(byte* out);
};