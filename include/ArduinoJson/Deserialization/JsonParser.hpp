// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../JsonBuffer.hpp"
#include "../JsonVariant.hpp"
#include "StringReader.hpp"
#include "StringWriter.hpp"

namespace ArduinoJson {
namespace Internals {

// Parse JSON string to create JsonArrays and JsonObjects
// This internal class is not indended to be used directly.
// Instead, use JsonBuffer.parseArray() or .parseObject()
template <typename TReader, typename TWriter>
class JsonParser {
 public:
  JsonParser(JsonBuffer *buffer, TReader reader, TWriter writer,
             uint8_t nestingLimit)
      : _buffer(buffer),
        _reader(reader),
        _writer(writer),
        _nestingLimit(nestingLimit) {}

  JsonArray &parseArray();
  JsonObject &parseObject();

  JsonVariant parseVariant() {
    JsonVariant result;
    parseAnythingTo(&result);
    return result;
  }

 private:
  JsonParser &operator=(const JsonParser &);  // non-copiable

  static bool eat(TReader &, char charToSkip);
  FORCE_INLINE bool eat(char charToSkip) {
    return eat(_reader, charToSkip);
  }

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
           isInRange(c, 'A', 'Z') || c == '+' || c == '-' || c == '.';
  }

  static inline bool isQuote(char c) {
    return c == '\'' || c == '\"';
  }

  JsonBuffer *_buffer;
  TReader _reader;
  TWriter _writer;
  uint8_t _nestingLimit;
};

template <typename TJsonBuffer, typename TString>
struct JsonParserBuilder {
  typedef typename Internals::StringTraits<TString>::Iterator InputIterator;
  typedef JsonParser<StringReader<InputIterator>, TJsonBuffer &> TParser;

  static TParser makeParser(TJsonBuffer *buffer, TString &json,
                            uint8_t nestingLimit) {
    return TParser(buffer, InputIterator(json), *buffer, nestingLimit);
  }
};

template <typename TJsonBuffer>
struct JsonParserBuilder<TJsonBuffer, char *> {
  typedef typename Internals::StringTraits<char *>::Iterator InputIterator;
  typedef JsonParser<StringReader<InputIterator>, StringWriter> TParser;

  static TParser makeParser(TJsonBuffer *buffer, char *json,
                            uint8_t nestingLimit) {
    return TParser(buffer, InputIterator(json), json, nestingLimit);
  }
};

template <typename TJsonBuffer, typename TString>
inline typename JsonParserBuilder<TJsonBuffer, TString>::TParser makeParser(
    TJsonBuffer *buffer, TString &json, uint8_t nestingLimit) {
  return JsonParserBuilder<TJsonBuffer, TString>::makeParser(buffer, json,
                                                             nestingLimit);
}
}
}
