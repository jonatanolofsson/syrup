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


#define __bswap_constant_16(x) \
     ((unsigned short int) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))

#define __bswap_constant_32(x) \
     ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |           \
      (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

#define htobe16(x) __bswap_constant_16 (x)
#define htole16(x) (x)
#define be16toh(x) __bswap_constant_16 (x)
#define le16toh(x) (x)

#define htobe32(x) __bswap_constant_32 (x)
#define htole32(x) (x)
#define be32toh(x) __bswap_constant_32 (x)
#define le32toh(x) (x)

#endif
