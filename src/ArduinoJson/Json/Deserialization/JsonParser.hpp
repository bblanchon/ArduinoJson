// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../../JsonError.hpp"
#include "../../JsonVariant.hpp"
#include "../../Memory/JsonBuffer.hpp"
#include "../../Strings/StringWriter.hpp"
#include "../../TypeTraits/IsConst.hpp"

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
  JsonError parse(JsonArray &destination);
  JsonError parse(JsonObject &destination);
  JsonError parse(JsonVariant &destination);

 private:
  JsonParser &operator=(const JsonParser &);  // non-copiable

  static bool eat(TReader &, char charToSkip);
  FORCE_INLINE bool eat(char charToSkip) {
    return eat(_reader, charToSkip);
  }

  const char *parseString();
  JsonError parseArray(JsonVariant &variant);
  JsonError parseObject(JsonVariant &variant);
  JsonError parseValue(JsonVariant &variant);

  static inline bool isBetween(char c, char min, char max) {
    return min <= c && c <= max;
  }

  static inline bool canBeInNonQuotedString(char c) {
    return isBetween(c, '0', '9') || isBetween(c, '_', 'z') ||
           isBetween(c, 'A', 'Z') || c == '+' || c == '-' || c == '.';
  }

  static inline bool isQuote(char c) {
    return c == '\'' || c == '\"';
  }

  JsonBuffer *_buffer;
  TReader _reader;
  TWriter _writer;
  uint8_t _nestingLimit;
};

template <typename TJsonBuffer, typename TString, typename Enable = void>
struct JsonParserBuilder {
  typedef typename StringTraits<TString>::Reader InputReader;
  typedef JsonParser<InputReader, TJsonBuffer &> TParser;

  static TParser makeParser(TJsonBuffer *buffer, TString &json,
                            uint8_t nestingLimit) {
    return TParser(buffer, InputReader(json), *buffer, nestingLimit);
  }
};

template <typename TJsonBuffer, typename TChar>
struct JsonParserBuilder<TJsonBuffer, TChar *,
                         typename EnableIf<!IsConst<TChar>::value>::type> {
  typedef typename StringTraits<TChar *>::Reader TReader;
  typedef StringWriter<TChar> TWriter;
  typedef JsonParser<TReader, TWriter> TParser;

  static TParser makeParser(TJsonBuffer *buffer, TChar *json,
                            uint8_t nestingLimit) {
    return TParser(buffer, TReader(json), TWriter(json), nestingLimit);
  }
};

template <typename TJsonBuffer, typename TString>
inline typename JsonParserBuilder<TJsonBuffer, TString>::TParser makeParser(
    TJsonBuffer *buffer, TString &json, uint8_t nestingLimit) {
  return JsonParserBuilder<TJsonBuffer, TString>::makeParser(buffer, json,
                                                             nestingLimit);
}
}  // namespace Internals
}  // namespace ArduinoJson
