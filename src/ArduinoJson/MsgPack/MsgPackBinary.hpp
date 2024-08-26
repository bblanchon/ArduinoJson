#pragma once

#include <ArduinoJson/Variant/Converter.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class MsgPackBinary {
 public:
  MsgPackBinary() : data_(nullptr), size_(0) {}
  explicit MsgPackBinary(const void* c, size_t size) : data_(c), size_(size) {}

  const void* data() const {
    return data_;
  }

  size_t size() const {
    return size_;
  }

 private:
  const void* data_;
  size_t size_;
};

template <>
struct Converter<MsgPackBinary> : private detail::VariantAttorney {
  static void toJson(MsgPackBinary src, JsonVariant dst) {
    auto data = VariantAttorney::getData(dst);
    if (!data)
      return;
    auto resources = getResourceManager(dst);
    data->clear(resources);
    if (src.data()) {
      size_t headerSize = src.size() >= 0x10000 ? 5
                          : src.size() >= 0x100 ? 3
                                                : 2;
      auto str = resources->createString(src.size() + headerSize);
      if (str) {
        resources->saveString(str);
        auto ptr = reinterpret_cast<uint8_t*>(str->data);
        switch (headerSize) {
          case 2:
            ptr[0] = uint8_t(0xc4);
            ptr[1] = uint8_t(src.size() & 0xff);
            break;
          case 3:
            ptr[0] = uint8_t(0xc5);
            ptr[1] = uint8_t(src.size() >> 8 & 0xff);
            ptr[2] = uint8_t(src.size() & 0xff);
            break;
          case 5:
            ptr[0] = uint8_t(0xc6);
            ptr[1] = uint8_t(src.size() >> 24 & 0xff);
            ptr[2] = uint8_t(src.size() >> 16 & 0xff);
            ptr[3] = uint8_t(src.size() >> 8 & 0xff);
            ptr[4] = uint8_t(src.size() & 0xff);
            break;
          default:
            ARDUINOJSON_ASSERT(false);
        }
        memcpy(ptr + headerSize, src.data(), src.size());
        data->setRawString(str);
        return;
      }
    }
  }

  static MsgPackBinary fromJson(JsonVariantConst src) {
    auto data = getData(src);
    if (!data)
      return {};
    auto rawstr = data->asRawString();
    auto p = reinterpret_cast<const uint8_t*>(rawstr.c_str());
    auto n = rawstr.size();
    if (n >= 2 && p[0] == 0xc4) {  // bin 8
      size_t size = p[1];
      if (size + 2 == n)
        return MsgPackBinary(p + 2, size);
    } else if (n >= 3 && p[0] == 0xc5) {  // bin 16
      size_t size = size_t(p[1] << 8) | p[2];
      if (size + 3 == n)
        return MsgPackBinary(p + 3, size);
    } else if (n >= 5 && p[0] == 0xc6) {  // bin 32
      size_t size =
          size_t(p[1] << 24) | size_t(p[2] << 16) | size_t(p[3] << 8) | p[4];
      if (size + 5 == n)
        return MsgPackBinary(p + 5, size);
    }
    return {};
  }

  static bool checkJson(JsonVariantConst src) {
    return fromJson(src).data() != nullptr;
  }
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
