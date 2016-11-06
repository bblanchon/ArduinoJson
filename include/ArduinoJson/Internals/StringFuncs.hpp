// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Configuration.hpp"

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#include <WString.h>
#endif

#if ARDUINOJSON_ENABLE_STD_STRING
#include <string>
#endif

namespace ArduinoJson {
namespace Internals {

template <typename TString>
struct StringFuncs {};

template <typename TString>
struct StringFuncs<const TString> : StringFuncs<TString> {};

template <typename TString>
struct StringFuncs<TString&> : StringFuncs<TString> {};

struct CharPtrFuncs {
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
  static const bool should_duplicate = false;
};

template <>
struct StringFuncs<const char*> : CharPtrFuncs {};

template <>
struct StringFuncs<char*> : CharPtrFuncs {};

template <size_t N>
struct StringFuncs<char[N]> : CharPtrFuncs {};

template <typename TString>
struct StdStringFuncs {
  template <typename Buffer>
  static char* duplicate(const TString& str, Buffer* buffer) {
    if (!str.c_str()) return NULL;  // <- Arduino string can return NULL
    size_t size = str.length() + 1;
    void* dup = buffer->alloc(size);
    if (dup != NULL) memcpy(dup, str.c_str(), size);
    return static_cast<char*>(dup);
  }

  static bool equals(const TString& str, const char* expected) {
    return str == expected;
  }

  static void append(TString& str, char c) {
    str += c;
  }

  static const bool has_append = true;
  static const bool should_duplicate = true;
};

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
template <>
struct StringFuncs<String> : StdStringFuncs<String> {};
template <>
struct StringFuncs<StringSumHelper> : StdStringFuncs<StringSumHelper> {};
#endif

#if ARDUINOJSON_ENABLE_STD_STRING
template <>
struct StringFuncs<std::string> : StdStringFuncs<std::string> {};
#endif
}
}
