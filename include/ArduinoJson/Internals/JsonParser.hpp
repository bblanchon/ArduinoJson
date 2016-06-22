// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../JsonBuffer.hpp"
#include "../JsonVariant.hpp"

namespace ArduinoJson {
namespace Internals {

// Parse JSON string to create JsonArrays and JsonObjects
// This internal class is not indended to be used directly.
// Instead, use JsonBuffer.parseArray() or .parseObject()
class JsonParser {
 public:
  JsonParser(JsonBuffer *buffer, char *json, uint8_t nestingLimit)
      : _buffer(buffer),
        _readPtr(json ? json : ""),
        _writePtr(json),
        _nestingLimit(nestingLimit) {}

  JsonArray &parseArray();
  JsonObject &parseObject();

  JsonVariant parseVariant() {
    JsonVariant result;
    parseAnythingTo(&result);
    return result;
  }

 private:
  bool skip(char charToSkip);

  const char *parseString();
  bool parseAnythingTo(JsonVariant *destination);
  FORCE_INLINE bool parseAnythingToUnsafe(JsonVariant *destination);

  inline bool parseArrayTo(JsonVariant *destination);
  inline bool parseObjectTo(JsonVariant *destination);
  inline bool parseStringTo(JsonVariant *destination);

  static inline bool isInRange(char c, char min, char max) {
    return min <= c && c <= max;
  }

  static inline bool isLetterOrNumber(char c) {
    return isInRange(c, '0', '9') || isInRange(c, 'a', 'z') ||
           isInRange(c, 'A', 'Z') || c == '-' || c == '.';
  }

  static inline bool isQuote(char c) {
    return c == '\'' || c == '\"';
  }

  JsonBuffer *_buffer;
  const char *_readPtr;
  char *_writePtr;
  uint8_t _nestingLimit;
};
}
}
