// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
struct IsCharOrVoid {
  static const bool value =
      is_same<T, void>::value || is_same<T, char>::value ||
      is_same<T, unsigned char>::value || is_same<T, signed char>::value;
};

template <typename T>
struct IsCharOrVoid<const T> : IsCharOrVoid<T> {};

template <typename TSource>
struct Reader<TSource*,
              typename enable_if<IsCharOrVoid<TSource>::value>::type> {
  const char* _ptr;

 public:
  explicit Reader(const void* ptr)
      : _ptr(ptr ? reinterpret_cast<const char*>(ptr) : "") {}

  int read() {
    return static_cast<unsigned char>(*_ptr++);
  }

  size_t readBytes(char* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) buffer[i] = *_ptr++;
    return length;
  }
};

template <typename TSource>
struct BoundedReader<TSource*,
                     typename enable_if<IsCharOrVoid<TSource>::value>::type>
    : public IteratorReader<const char*> {
 public:
  explicit BoundedReader(const void* ptr, size_t len)
      : IteratorReader<const char*>(reinterpret_cast<const char*>(ptr),
                                    reinterpret_cast<const char*>(ptr) + len) {}
};

}  // namespace ARDUINOJSON_NAMESPACE
