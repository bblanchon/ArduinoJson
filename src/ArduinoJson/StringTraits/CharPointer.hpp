// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename TChar>
struct CharPointerTraits {
  class Reader {
    const TChar* _ptr;

   public:
    Reader(const TChar* ptr)
        : _ptr(ptr ? ptr : reinterpret_cast<const TChar*>("")) {}

    void move() {
      ++_ptr;
    }

    char current() const {
      return char(_ptr[0]);
    }

    char next() const {
      return char(_ptr[1]);
    }
  };

  static bool equals(const TChar* str, const char* expected) {
    return strcmp(reinterpret_cast<const char*>(str), expected) == 0;
  }

  // TODO: remove
  template <typename Buffer>
  static char* duplicate(const TChar* str, Buffer* buffer) {
    if (!str) return NULL;
    size_t size = strlen(reinterpret_cast<const char*>(str)) + 1;
    void* dup = buffer->alloc(size);
    if (dup != NULL) memcpy(dup, str, size);
    return static_cast<char*>(dup);
  }

  static const bool has_append = false;
  static const bool has_equals = true;
};

// const char*, const unsigned char*, const signed char*
template <typename TChar>
struct StringTraits<TChar*, typename TypeTraits::EnableIf<
                                TypeTraits::IsChar<TChar>::value &&
                                TypeTraits::IsConst<TChar>::value>::type>
    : CharPointerTraits<TChar> {
  // Just save the pointer
  template <typename Buffer, typename Destination>
  static typename TypeTraits::EnableIf<TypeTraits::IsConst<TChar>::value,
                                       bool>::type
  save(const TChar* source, Destination& dest, Buffer*) {
    dest = reinterpret_cast<const char*>(source);
    return true;
  }
};

// char*, unsigned char*, signed char*
template <typename TChar>
struct StringTraits<TChar*, typename TypeTraits::EnableIf<
                                TypeTraits::IsChar<TChar>::value &&
                                !TypeTraits::IsConst<TChar>::value>::type>
    : CharPointerTraits<TChar> {
  // Make a copy of the string
  template <typename Buffer, typename Destination>
  static typename TypeTraits::EnableIf<!TypeTraits::IsConst<TChar>::value,
                                       bool>::type
  save(const TChar* source, Destination& dest, Buffer* buffer) {
    if (source) {
      size_t size = strlen(reinterpret_cast<const char*>(source)) + 1;
      void* dup = buffer->alloc(size);
      if (!dup) return false;
      memcpy(dup, source, size);
      dest = reinterpret_cast<const char*>(dup);
    } else {
      dest = reinterpret_cast<const char*>(source);
    }
    return true;
  }
};
}
}
