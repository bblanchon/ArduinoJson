#pragma once

#include <ArduinoJson/Variant/Converter.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class MsgPackExtension {
 public:
  MsgPackExtension() : data_(nullptr), size_(0), type_(0) {}
  explicit MsgPackExtension(int8_t type, const void* data, size_t size)
      : data_(data), size_(size), type_(type) {}

  int8_t type() const {
    return type_;
  }

  const void* data() const {
    return data_;
  }

  size_t size() const {
    return size_;
  }

 private:
  const void* data_;
  size_t size_;
  int8_t type_;
};

template <>
struct Converter<MsgPackExtension> : private detail::VariantAttorney {
  static void toJson(MsgPackExtension src, JsonVariant dst) {
    auto data = VariantAttorney::getData(dst);
    if (!data)
      return;
    auto resources = getResourceManager(dst);
    if (src.data()) {
      uint8_t format, sizeBytes;
      if (src.size() >= 0x10000) {
        format = 0xc9;  // ext 32
        sizeBytes = 4;
      } else if (src.size() >= 0x100) {
        format = 0xc8;  // ext 16
        sizeBytes = 2;
      } else if (src.size() == 16) {
        format = 0xd8;  // fixext 16
        sizeBytes = 0;
      } else if (src.size() == 8) {
        format = 0xd7;  // fixext 8
        sizeBytes = 0;
      } else if (src.size() == 4) {
        format = 0xd6;  // fixext 4
        sizeBytes = 0;
      } else if (src.size() == 2) {
        format = 0xd5;  // fixext 2
        sizeBytes = 0;
      } else if (src.size() == 1) {
        format = 0xd4;  // fixext 1
        sizeBytes = 0;
      } else {
        format = 0xc7;  // ext 8
        sizeBytes = 1;
      }

      auto str = resources->createString(src.size() + 2 + sizeBytes);
      if (str) {
        resources->saveString(str);
        auto ptr = reinterpret_cast<uint8_t*>(str->data);
        *ptr++ = uint8_t(format);
        for (uint8_t i = 0; i < sizeBytes; i++)
          *ptr++ = uint8_t(src.size() >> (sizeBytes - i - 1) * 8 & 0xff);
        *ptr++ = uint8_t(src.type());
        memcpy(ptr, src.data(), src.size());
        data->setRawString(str);
        return;
      }
    }
    data->setNull();
  }

  static MsgPackExtension fromJson(JsonVariantConst src) {
    auto data = getData(src);
    if (!data)
      return {};
    auto rawstr = data->asRawString();
    if (rawstr.size() == 0)
      return {};
    auto p = reinterpret_cast<const uint8_t*>(rawstr.c_str());

    size_t payloadSize = 0;
    uint8_t headerSize = 0;

    const uint8_t& code = p[0];

    if (code >= 0xd4 && code <= 0xd8) {  // fixext 1
      headerSize = 2;
      payloadSize = size_t(1) << (code - 0xd4);
    }

    if (code >= 0xc7 && code <= 0xc9) {
      uint8_t sizeBytes = uint8_t(1 << (code - 0xc7));
      for (uint8_t i = 0; i < sizeBytes; i++)
        payloadSize = (payloadSize << 8) | p[1 + i];
      headerSize = uint8_t(2 + sizeBytes);
    }

    if (rawstr.size() == headerSize + payloadSize)
      return MsgPackExtension(int8_t(p[headerSize - 1]), p + headerSize,
                              payloadSize);

    return {};
  }

  static bool checkJson(JsonVariantConst src) {
    return fromJson(src).data() != nullptr;
  }
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
