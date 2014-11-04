// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../JsonBuffer.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonParser {
 public:
  JsonParser(JsonBuffer *buffer, char *json) : _buffer(buffer), _ptr(json) {}

  JsonArray &parseArray();
  JsonObject &parseObject();

 private:
  bool isEnd() { return *_ptr == '\0'; }

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
};
}
}
