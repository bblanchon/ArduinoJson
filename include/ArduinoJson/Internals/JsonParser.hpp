// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

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

 private:
  bool skip(char charToSkip);

  const char *parseString();
  bool parseAnythingTo(JsonVariant *destination);
  FORCE_INLINE bool parseAnythingToUnsafe(JsonVariant *destination);

  inline bool parseArrayTo(JsonVariant *destination);
  inline bool parseObjectTo(JsonVariant *destination);
  inline bool parseStringTo(JsonVariant *destination);

  JsonBuffer *_buffer;
  const char *_readPtr;
  char *_writePtr;
  uint8_t _nestingLimit;
};
}
}
