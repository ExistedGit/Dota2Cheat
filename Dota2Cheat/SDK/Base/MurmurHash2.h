#pragma once
#include <string_view>

inline constexpr unsigned int MurmurHash2(std::string_view key)
{
	const unsigned int m = 0x5bd1e995;
	const unsigned int seed = 0;
	const int r = 24;
	auto len = key.size();

	unsigned int h = 0x3141592E + len % 4;

	const char* data = key.data();
	unsigned int k = 0;

	while (len >= 4)
	{
		k = data[0];
		k |= data[1] << 8;
		k |= data[2] << 16;
		k |= data[3] << 24;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	switch (len)
	{
	case 3:
		h ^= data[2] << 16;
	case 2:
		h ^= data[1] << 8;
	case 1:
		h ^= data[0];
		h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}