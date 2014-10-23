#pragma once

#include "JsonNode.hpp"

namespace ArduinoJson {
class JsonBuffer;

namespace Internals {
class JsonNode;

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