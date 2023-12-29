#include "DXT5Decoder.h"

void DXT5Decoder::DecompressBlockDXT1(int x, int y, int width, uint8_t blockStorage[8], byte* pixels, int stride)
{
	auto color0 = (uint16_t)(blockStorage[0] | blockStorage[1] << 8);
	auto color1 = (uint16_t)(blockStorage[2] | blockStorage[3] << 8);

	uint8_t r0, g0, b0, r1, g1, b1;

	ConvertRgb565ToRgb888(color0, r0, g0, b0);
	ConvertRgb565ToRgb888(color1, r1, g1, b1);

	uint32_t c1 = blockStorage[4];
	auto c2 = (uint32_t)blockStorage[5] << 8;
	auto c3 = (uint32_t)blockStorage[6] << 16;
	auto c4 = (uint32_t)blockStorage[7] << 24;
	auto code = c1 | c2 | c3 | c4;

	for (auto j = 0; j < 4; j++)
	{
		for (auto i = 0; i < 4; i++)
		{
			auto positionCode = (byte)((code >> (2 * ((4 * j) + i))) & 0x03);

			byte finalR = 0, finalG = 0, finalB = 0;

			switch (positionCode)
			{
			case 0:
				finalR = r0;
				finalG = g0;
				finalB = b0;
				break;
			case 1:
				finalR = r1;
				finalG = g1;
				finalB = b1;
				break;
			case 2:
				if (color0 > color1)
				{
					finalR = (byte)(((2 * r0) + r1) / 3);
					finalG = (byte)(((2 * g0) + g1) / 3);
					finalB = (byte)(((2 * b0) + b1) / 3);
				}
				else
				{
					finalR = (byte)((r0 + r1) / 2);
					finalG = (byte)((g0 + g1) / 2);
					finalB = (byte)((b0 + b1) / 2);
				}

				break;
			case 3:
				if (color0 < color1)
				{
					break;
				}

				finalR = (byte)(((2 * r1) + r0) / 3);
				finalG = (byte)(((2 * g1) + g0) / 3);
				finalB = (byte)(((2 * b1) + b0) / 3);
				break;
			}

			auto pixelIndex = ((y + j) * stride) + ((x + i) * 4);

			if (x + i < width && outSize > pixelIndex + 3)
			{
				pixels[pixelIndex] = finalB;
				pixels[pixelIndex + 1] = finalG;
				pixels[pixelIndex + 2] = finalR;
				pixels[pixelIndex + 3] = UINT8_MAX;
			}
		}
	}
}

void DXT5Decoder::Decompress8BitBlock(int bx, int w, int offset, uint64_t block, byte* pixels, int stride)
{
	auto e0 = (byte)(block & 0xFF);
	auto e1 = (byte)(block >> 8 & 0xFF);
	auto code = block >> 16;

	for (auto y = 0; y < 4; y++)
	{
		for (auto x = 0; x < 4; x++)
		{
			auto dataIndex = offset + (y * stride) + (x * 4);

			uint32_t index = (byte)(code & 0x07);
			code >>= 3;

			if (bx + x >= w || outSize <= dataIndex)
				continue;

			if (index == 0)
				pixels[dataIndex] = e0;
			else if (index == 1)
				pixels[dataIndex] = e1;
			else
			{
				if (e0 > e1)
					pixels[dataIndex] = (byte)((((8 - index) * e0) + ((index - 1) * e1)) / 7);
				else
				{
					if (index == 6)
						pixels[dataIndex] = 0;
					else if (index == 7)
						pixels[dataIndex] = 255;
					else
						pixels[dataIndex] = (byte)((((6 - index) * e0) + ((index - 1) * e1)) / 5);
				}
			}
		}
	}
}

void DXT5Decoder::Decode(byte* out)
{
	bool yCoCg = true;

	auto offset = 0;
	auto blockCountX = (blockWidth + 3) / 4;
	auto blockCountY = (blockHeight + 3) / 4;
	auto imageWidth = w;
	auto rowBytes = w * 4;

	auto data = out;

	for (auto j = 0; j < blockCountY; j++)
	{
		for (auto i = 0; i < blockCountX; i++)
		{
			auto blockAlpha = *(uint64_t*)(input + offset); // TODO: Can we work on bytes directly here?
			offset += 8;
			auto blockStorage = *(uint64_t*)(input + offset);
			offset += 8;
			auto ofs = (i * 16) + (j * 4 * rowBytes);
			DecompressBlockDXT1(i * 4, j * 4, imageWidth, (uint8_t*)&blockStorage, data, rowBytes);
			Decompress8BitBlock(i * 4, imageWidth, ofs + 3, blockAlpha, data, rowBytes);

			for (auto y = 0; y < 4; y++)
			{
				for (auto x = 0; x < 4; x++)
				{
					auto dataIndex = ofs + (x * 4) + (y * rowBytes);
					if ((i * 4) + x >= imageWidth || outSize < dataIndex + 3)
					{
						break;
					}

					if (yCoCg)
					{
						auto s = (data[dataIndex + 0] >> 3) + 1;
						auto cg = (data[dataIndex + 1] - 128) / s;
						auto co = (data[dataIndex + 2] - 128) / s;
						auto& a = data[dataIndex + 3];

						data[dataIndex + 0] = ClampColor(a - co - cg);
						data[dataIndex + 1] = ClampColor(a + cg);
						data[dataIndex + 2] = ClampColor(a + co - cg);
						a = 255;
					}
				}
			}
		}
	}
}
