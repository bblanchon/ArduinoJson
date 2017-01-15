// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {

struct CharPointerTraits {
  class Iterator {
    const char* _ptr;

   public:
    Iterator(const char* ptr) : _ptr(ptr ? ptr : "") {}

    char next() {
      char c = *_ptr;
      if (c) ++_ptr;
      return c;
    }
  };

  static bool equals(const char* str, const char* expected) {
    return strcmp(str, expected) == 0;
  }

  template <typename Buffer>
  static char* duplicate(const char* str, Buffer* buffer) {
    if (!str) return NULL;
    size_t size = strlen(str) + 1;
    void* dup = buffer->alloc(size);
    if (dup != NULL) memcpy(dup, str, size);
    return static_cast<char*>(dup);
  }

  static const bool has_append = false;
  static const bool has_equals = true;
  static const bool should_duplicate = false;
};

template <>
struct StringTraits<const char*, void> : CharPointerTraits {};

template <>
struct StringTraits<char*, void> : CharPointerTraits {};
}
}
