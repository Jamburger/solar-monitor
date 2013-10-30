#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>

template <typename T>
struct Message {
  uint8_t type;
  uint32_t seq;
  T value;
};

#endif /* MESSAGE_H */
