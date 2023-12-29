#pragma once
#include "../../pch.h"
#include <streambuf>
#include <istream>
#include <string_view>
#include "vtex.h"
#include <stb_image.h>
#include "DXT5Decoder.h"

// Code ported from https://github.com/ValveResourceFormat/ValveResourceFormat

struct memstream {
	char* data{};
	size_t pos = 0;

	memstream(char* data) : data(data) {

	}

	template<typename T>
	memstream& operator>>(T& v) {
		v = *(T*)(data + pos);
		pos += sizeof(T);
		return *this;
	}

	template<typename T = char>
	memstream& read(T* v, int n) {
		memcpy(v, data + pos, n * sizeof(T));
		pos += sizeof(T) * n;
		return *this;
	}

	void seekg(size_t s) {
		pos += s;
	}
};


class VTexParser {
public:
	struct ImageData {
		int w, h;
		unsigned char* data;
	};

	static ImageData Load(std::string_view filename);
};