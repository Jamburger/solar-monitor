#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <math.h>

void copy_big_endian(void * const dst, const void * const src, size_t len) {
  int test = 1;
  char * const cdst = (char * const)dst;
  const char * const csrc = (const char * const)src;
  if (((char *)(&test))[0]) {
    // Little endian.
    for (size_t i = 0; i < len; ++i) {
      cdst[i] = csrc[len - i - 1];
    }
  } else {
    // Big endian.
    for (size_t i = 0; i < len; ++i) {
      cdst[i] = csrc[i];
    }
  }
}

template <typename FloatType, typename FixedType>
FixedType float_to_fixed(FloatType float_value, unsigned int decimal_digits) {
  return (FixedType) (float_value * pow(10, decimal_digits));
}

#endif /* UTIL_H */
