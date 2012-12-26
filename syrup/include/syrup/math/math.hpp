#ifndef SYRUP_MATH_HPP
#define SYRUP_MATH_HPP

#include <cmath>
#include <algorithm>
#include <stdint.h>

#define clamp(mn, val, mx) ((val)<(mn)?(mn):((val)>(mx)?(mx):(val)))
#define sign(val)   ((val)<0?-1:1)
#define heaviside(val)   ((val)<0?0:1)
using std::min;
using std::max;
using std::abs;


inline uint32_t convert_endian(const int8_t _1, const int8_t _2, const int8_t _3, const int8_t _4)
{
    return _4 << 24 | _3 << 16 | _2 << 8 | _1;
}
inline uint32_t convert_endian(const int8_t _1, const int8_t _2, const int8_t _3)
{
    return _3 << 24 | _2 << 16 | _1 << 8;
}
inline uint16_t convert_endian(const int8_t _1, const int8_t _2)
{
    return _2 << 8 | _1;
}

#endif
