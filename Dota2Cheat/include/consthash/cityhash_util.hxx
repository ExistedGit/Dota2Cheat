
#pragma once
#ifndef _CONSTHASH_CITYHASH_UTIL_HXX
#define _CONSTHASH_CITYHASH_UTIL_HXX

#include "./common.hxx"
#include "./u128.hxx"

CONSTHASH_NAMESPACE_BEGIN;

namespace __detail {

typedef rtu128_t uint128_t;

constexpr uint64_t Uint128Low64(const uint128_t& x) { return x.first; }
constexpr uint64_t Uint128High64(const uint128_t& x) { return x.second; }

// Some primes between 2^63 and 2^64 for various uses.
static const uint64_t k0 = 0xc3a5c85c97cb3127ULL;
static const uint64_t k1 = 0xb492b66fbe98f273ULL;
static const uint64_t k2 = 0x9ae16a3b2f90404fULL;

// Magic numbers for 32-bit hashing.  Copied from Murmur3.
static const uint32_t c1 = 0xcc9e2d51;
static const uint32_t c2 = 0x1b873593;

constexpr uint64_t swap_bytes_64(uint64_t value)
{
  return (value & 0x00000000000000FFULL) << 56 | (value & 0x000000000000FF00ULL) << 40 |
		 (value & 0x0000000000FF0000ULL) << 24 | (value & 0x00000000FF000000ULL) << 8 |
		 (value & 0x000000FF00000000ULL) >> 8 | (value & 0x0000FF0000000000ULL) >> 24 |
		 (value & 0x00FF000000000000ULL) >> 40 | (value & 0xFF00000000000000ULL) >> 56;
}

constexpr uint32_t swap_bytes_32(uint32_t value)
{
  return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |
		 (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}

// value ^= value >> rot;
template<typename T>
constexpr T xor_shr(T value, int rot)
{
	return value ^ (value >> rot);
}

constexpr uint64_t Hash128to64(uint128_t x) {
  // const uint64_t kMul = 0x9ddfea08eb382d69ULL;
  // uint64_t a = (Uint128Low64(x) ^ Uint128High64(x)) * kMul;
  // a ^= (a >> 47);
  // uint64_t b = (Uint128High64(x) ^ a) * kMul;
  // b ^= (b >> 47);
  // b *= kMul;
  // return b;
  return xor_shr((Uint128High64(x) ^ xor_shr((Uint128Low64(x) ^ Uint128High64(x)) * 0x9ddfea08eb382d69ULL, 47)) * 0x9ddfea08eb382d69ULL, 47) * 0x9ddfea08eb382d69ULL;
}

// A 32-bit to 32-bit integer hash copied from Murmur3.
constexpr uint32_t fmix(uint32_t h)
{
  // h ^= h >> 16;
  // h *= 0x85ebca6b;
  // h ^= h >> 13;
  // h *= 0xc2b2ae35;
  // h ^= h >> 16;
  return xor_shr<uint32_t>(xor_shr<uint32_t>(xor_shr<uint32_t>(h, 16) * 0x85ebca6b, 13) * 0xc2b2ae35, 16);
}

constexpr uint64_t Fetch64(const char *p) {
  return
	(static_cast<uint64_t>((uint8_t)p[0])) |
	(static_cast<uint64_t>((uint8_t)p[1]) << 8) |
	(static_cast<uint64_t>((uint8_t)p[2]) << 16) |
	(static_cast<uint64_t>((uint8_t)p[3]) << 24) |
	(static_cast<uint64_t>((uint8_t)p[4]) << 32) |
	(static_cast<uint64_t>((uint8_t)p[5]) << 40) |
	(static_cast<uint64_t>((uint8_t)p[6]) << 48) |
	(static_cast<uint64_t>((uint8_t)p[7]) << 56);
}

constexpr uint32_t Fetch32(const char *p) {
  return
	(static_cast<uint32_t>((uint8_t)p[0])) |
	(static_cast<uint32_t>((uint8_t)p[1]) << 8) |
	(static_cast<uint32_t>((uint8_t)p[2]) << 16) |
	(static_cast<uint32_t>((uint8_t)p[3]) << 24);
}

constexpr uint32_t Rotate32(uint32_t val, int shift) {
  // Avoid shifting by 32: doing so yields an undefined result.
  return shift == 0 ? val : ((val >> shift) | (val << (32 - shift)));
}

// Bitwise right rotate.  Normally this will compile to a single
// instruction, especially if the shift is a manifest constant.
constexpr uint64_t Rotate(uint64_t val, int shift) {
  // Avoid shifting by 64: doing so yields an undefined result.
  return shift == 0 ? val : ((val >> shift) | (val << (64 - shift)));
}

constexpr uint64_t ShiftMix(uint64_t val) {
  return val ^ (val >> 47);
}

constexpr uint64_t HashLen16(uint64_t u, uint64_t v) {
  return Hash128to64(uint128_t(u, v));
}

constexpr uint64_t HashLen16(uint64_t u, uint64_t v, uint64_t mul) {
  // uint64_t a = (u ^ v) * mul;
  // a ^= (a >> 47);
  // uint64_t b = (v ^ a) * mul;
  // b ^= (b >> 47);
  // b *= mul;
  // return b;
  return xor_shr((v ^ xor_shr((u ^ v) * mul, 47)) * mul, 47) * mul;
}

constexpr uint32_t Mur(uint32_t a, uint32_t h) {
  // Helper from Murmur3 for combining two 32-bit values.
  return Rotate32(h ^ (Rotate32(a * c1, 17) * c2), 19) * 5 + 0xe6546b64;
}

// Return a 16-byte hash for 48 bytes.  Quick and dirty.
// Callers do best to use "random-looking" values for a and b.
constexpr uint128_t WeakHashLen32WithSeeds(
	uint64_t w, uint64_t x, uint64_t y, uint64_t z, uint64_t a, uint64_t b) {
  return uint128_t(a + w + y + x + z, Rotate(b + a + w + z, 21) + Rotate(a + w + x + y, 44) + a + w);
}

// Return a 16-byte hash for s[0] ... s[31], a, and b.  Quick and dirty.
constexpr uint128_t WeakHashLen32WithSeeds(
	const char* s, uint64_t a, uint64_t b) {
  return WeakHashLen32WithSeeds(Fetch64(s),
								Fetch64(s + 8),
								Fetch64(s + 16),
								Fetch64(s + 24),
								a,
								b);
}

constexpr uint64_t HashLen0to16_ge_8_sub(const char* s, size_t len, uint64_t mul, uint64_t a, uint64_t b) {
	return HashLen16(Rotate(b, 37) * mul + a, (Rotate(a, 25) + b) * mul, mul);
}

constexpr uint64_t HashLen0to16_ge_8(const char* s, size_t len) {
	return HashLen0to16_ge_8_sub(s, len, k2 + len * 2, Fetch64(s) + k2, Fetch64(s + len - 8));
}

constexpr uint64_t HashLen0to16_ge_4(const char* s, size_t len) {
	return HashLen16(len + (static_cast<uint64_t>(Fetch32(s)) << 3), Fetch32(s + len - 4), k2 + len * 2);
}

constexpr uint64_t HashLen0to16_gt_0(const char* s, size_t len) {
	return ShiftMix((static_cast<uint32_t>((uint8_t)s[0]) + (static_cast<uint32_t>((uint8_t)s[len >> 1]) << 8)) * k2 ^ (len + (static_cast<uint32_t>((uint8_t)s[len - 1]) << 2)) * k0) * k2;
}

constexpr uint64_t HashLen0to16(const char *s, size_t len) {
	return
		len >= 8 ? HashLen0to16_ge_8(s, len) :
		(len >= 4 ? HashLen0to16_ge_4(s, len) :
		(len > 0 ? HashLen0to16_gt_0(s, len) : k2));
}

constexpr uint64_t HashLen17to32_sub2(const char *s, size_t len, uint64_t a, uint64_t b, uint64_t c, uint64_t mul) {
  return HashLen16(Rotate(a + b, 43) + Rotate(c, 30) + Fetch64(s + len - 16) * k2,
				   a + Rotate(b + k2, 18) + c, mul);
}

constexpr uint64_t HashLen17to32_sub(const char *s, size_t len, uint64_t a, uint64_t b, uint64_t mul) {
	return HashLen17to32_sub2(s, len, a, b, Fetch64(s + len - 8) * mul, mul);
}

constexpr uint64_t HashLen17to32(const char *s, size_t len) {
  return HashLen17to32_sub(s, len, Fetch64(s) * k1, Fetch64(s + 8), k2 + len * 2);
}

constexpr uint64_t HashLen33to64_sub3(const char *s, size_t len, uint64_t mul,
									uint64_t a, uint64_t b, uint64_t c, uint64_t d,
									uint64_t e, uint64_t f, uint64_t g, uint64_t h,
									uint64_t v, uint64_t y, uint64_t x, uint64_t z) {
  return ShiftMix((z + (swap_bytes_64((x + z) * mul + y) + b)) * mul + d + h) * mul + x;
}

constexpr uint64_t HashLen33to64_sub2(const char *s, size_t len, uint64_t mul,
									uint64_t a, uint64_t b, uint64_t c, uint64_t d,
									uint64_t e, uint64_t f, uint64_t g, uint64_t h,
									uint64_t v) {
  return HashLen33to64_sub3(s, len, mul, a, b, c, d, e, f, g, h, v,
	(swap_bytes_64((v + (swap_bytes_64(((Rotate(a + g, 43) + (Rotate(b, 30) + c) * 9) + v) * mul) + h)) * mul) + g) * mul,
	Rotate(e + f, 42) + c,
	e + f + c);
}

constexpr uint64_t HashLen33to64_sub1(const char *s, size_t len, uint64_t mul,
									uint64_t a, uint64_t b, uint64_t c, uint64_t d,
									uint64_t e, uint64_t f, uint64_t g) {
  return HashLen33to64_sub2(s, len, mul, a, b, c, d, e, f, g, Fetch64(s + len - 16) * mul, ((a + g) ^ d) + f + 1);
}

constexpr uint64_t HashLen33to64(const char *s, size_t len) {
  return HashLen33to64_sub1(s, len, k2 + len * 2,
							Fetch64(s) * k2, Fetch64(s + 8), Fetch64(s + len - 24), Fetch64(s + len - 32),
							Fetch64(s + 16) * k2, Fetch64(s + 24) * 9, Fetch64(s + len - 8));
}

} // namespace __detail

CONSTHASH_NAMESPACE_END;

#endif // _CONSTHASH_CITYHASH_UTIL_HXX
