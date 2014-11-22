// Copyright Benoit Blanchon 2014
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
      : _buffer(buffer), _ptr(json), _nestingLimit(nestingLimit) {}

  JsonArray &parseArray();
  JsonObject &parseObject();

 private:
  bool skip(char charToSkip);
  bool skip(const char *wordToSkip);
  void skipSpaces();

  void parseAnythingTo(JsonVariant &destination);
  inline void parseBooleanTo(JsonVariant &destination);
  inline void parseNullTo(JsonVariant &destination);
  inline void parseNumberTo(JsonVariant &destination);
  inline const char *parseString();

  JsonBuffer *_buffer;
  char *_ptr;
  uint8_t _nestingLimit;
};
}
}
