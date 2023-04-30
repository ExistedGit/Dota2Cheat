
#pragma once
#ifndef _CONSTHASH_U128_HXX
#define _CONSTHASH_U128_HXX

#include <cstdint>
#include <utility>
#include "./common.hxx"

CONSTHASH_NAMESPACE_BEGIN;

namespace __detail {;

typedef std::pair<uint64_t, uint64_t> rtu128_t;

template<uint64_t _Lo, uint64_t _Hi>
struct ctu128_t
{
	static constexpr uint64_t lo_val() { return _Lo; }
	static constexpr uint64_t hi_val() { return _Hi; }
	static constexpr rtu128_t rt_val() { return rtu128_t(lo_val(), hi_val()); }
};

#define RT2CT128_T(RT) __detail::ctu128_t<RT.first, RT.second>
#define CT2RT128(CT) (CT::rt_val())

}; // namespace __detail

CONSTHASH_NAMESPACE_END;

#endif
