/*******************************************************************************
 * tlx/math/popcount.hpp
 *
 * popcount() population count number of one bits - mainly for portability.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_POPCOUNT_HEADER
#define TLX_MATH_POPCOUNT_HEADER

#include <cstdint>

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace tlx {

//! \addtogroup tlx_math
//! \{

/******************************************************************************/
// popcount() - count one bits

//! popcount (count one bits) - generic SWAR implementation
static inline unsigned popcount_generic8(uint16_t x) {
    x = x - ((x >> 1) & 0x55);
    x = (x & 0x33) + ((x >> 2) & 0x33);
    return static_cast<uint8_t>((x + (x >> 4)) & 0x0F);
}

//! popcount (count one bits) - generic SWAR implementation
static inline unsigned popcount_generic16(uint16_t x) {
    x = x - ((x >> 1) & 0x5555);
    x = (x & 0x3333) + ((x >> 2) & 0x3333);
    return static_cast<uint16_t>(((x + (x >> 4)) & 0x0F0F) * 0x0101) >> 8;
}

//! popcount (count one bits) -
//! generic SWAR implementation from https://stackoverflow.com/questions/109023
static inline unsigned popcount_generic32(uint32_t x) {
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    return (((x + (x >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

//! popcount (count one bits) - generic SWAR implementation
static inline unsigned popcount_generic64(uint64_t x) {
    x = x - ((x >> 1) & 0x5555555555555555);
    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
    return (((x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F) * 0x0101010101010101) >> 56;
}

/******************************************************************************/

#if defined(__GNUC__) || defined(__clang__)

//! popcount (count one bits)
static inline unsigned popcount(unsigned i) {
    return static_cast<unsigned>(__builtin_popcount(i));
}

//! popcount (count one bits)
static inline unsigned popcount(int i) {
    return popcount(static_cast<unsigned>(i));
}

//! popcount (count one bits)
static inline unsigned popcount(unsigned long i) {
    return static_cast<unsigned>(__builtin_popcountl(i));
}

//! popcount (count one bits)
static inline unsigned popcount(long i) {
    return popcount(static_cast<unsigned long>(i));
}

//! popcount (count one bits)
static inline unsigned popcount(unsigned long long i) {
    return static_cast<unsigned>(__builtin_popcountll(i));
}

//! popcount (count one bits)
static inline unsigned popcount(long long i) {
    return popcount(static_cast<unsigned long long>(i));
}

#elif defined(_MSC_VER)

//! popcount (count one bits)
template <typename Integral>
inline unsigned popcount(Integral i) {
    unsigned long leading_zeros = 0;
    if (sizeof(i) > 4)
        return __popcnt(i);
    else
        return __popcnt64(i);
}

#else

//! popcount (count one bits)
template <typename Integral>
inline unsigned popcount(Integral i) {
    if (sizeof(i) == 1)
        return popcount_generic8(i);
    else if (sizeof(i) == 2)
        return popcount_generic16(i);
    else if (sizeof(i) == 4)
        return popcount_generic32(i);
    else if (sizeof(i) == 8)
        return popcount_generic64(i);
    else
        abort();
}

#endif

//! \}

} // namespace tlx

#endif // !TLX_MATH_POPCOUNT_HEADER

/******************************************************************************/