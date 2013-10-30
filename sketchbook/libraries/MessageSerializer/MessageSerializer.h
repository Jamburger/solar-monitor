#ifndef MESSAGE_SERIALIZER_H
#define MESSAGE_SERIALIZER_H

#include "../Message/Message.h"
#include "../util/util.h"

#include "Stream.h"

#include <stdint.h>

class MessageSerializer {
private:
  static const char SYNC_CHAR = 0xA7;

public:
  MessageSerializer(Stream &stream) : stream(stream) {
  }

  template <typename T>
  bool write(Message<T> &message) {
    // Prepare buffer with type, sequence number, and message contents.
    char str[1 + sizeof(message.seq) + sizeof(message.value)];

    // All fields are represented in network (big-endian) order.
    str[0] = message.type;
    copy_big_endian(str + 1, &message.seq, sizeof(message.seq));
    copy_big_endian(str + 5, &message.value, sizeof(message.value));

    return writeDatagram(str, sizeof(str));
  }

private:
  bool writeDatagram(const char * buf, size_t len) {
    if (len > 255) {
      return false;
    }

    // Prepare header and checksum. The checksum is the 8-bit two's complement
    // sum of every byte from the length to the last byte of the message value.
    char header[2] = {SYNC_CHAR, len};
    char checksum = len;
    for (size_t i = 0; i < len; ++i) {
      checksum += buf[i];
    }

    // Write the frame. The length field only includes the payload length, not
    // the length byte or checksum byte.
    return blockingWrite(header, sizeof(header)) &&
           blockingWrite(buf, len) &&
           blockingWrite(&checksum, 1);
  }

  bool blockingWrite(const char * buf, size_t len) {
    while (len) {
      size_t nwritten = stream.write((const uint8_t *)buf, len);
      buf += nwritten;
      len -= nwritten;
    }
    return true;
  }

  Stream &stream;
};

#endif /* MESSAGE_SERIALIZER_H */
