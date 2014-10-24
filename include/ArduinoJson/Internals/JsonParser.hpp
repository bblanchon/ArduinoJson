// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../ForwardDeclarations.hpp"
#include "JsonNode.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonParser {
 public:
  JsonParser(JsonBuffer *buffer, char *json) : _buffer(buffer), _ptr(json) {}

  JsonNode *parseAnything();

 private:
  JsonBuffer *_buffer;
  char *_ptr;

  bool isEnd() { return *_ptr == 0; }

  bool skip(char charToSkip);
  void skipSpaces();

  inline JsonNode *parseArray();
  inline JsonNode *parseBoolean();
  inline JsonNode *parseNull();
  inline JsonNode *parseNumber();
  inline JsonNode *parseObject();
  inline JsonNode *parseObjectKeyValue();
  inline JsonNode *parseString();
};
}
}
