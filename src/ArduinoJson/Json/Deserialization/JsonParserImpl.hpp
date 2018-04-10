// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Encoding.hpp"
#include "Comments.hpp"
#include "JsonParser.hpp"

template <typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonParser<TReader, TWriter>::eat(
    TReader &reader, char charToSkip) {
  skipSpacesAndComments(reader);
  if (reader.current() != charToSkip) return false;
  reader.move();
  return true;
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonError
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parse(JsonArray &array) {
  if (_nestingLimit == 0) return JsonError::TooDeep;

  // Check opening braket
  if (!eat('[')) return JsonError::OpeningBracketExpected;
  if (eat(']')) return JsonError::Ok;

  // Read each value
  for (;;) {
    // 1 - Parse value
    JsonVariant value;
    _nestingLimit--;
    JsonError error = parse(value);
    _nestingLimit++;
    if (error != JsonError::Ok) return error;
    if (!array.add(value)) return JsonError::NoMemory;

    // 2 - More values?
    if (eat(']')) return JsonError::Ok;
    if (!eat(',')) return JsonError::ClosingBracketExpected;
  }
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonError
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parse(
    JsonObject &object) {
  if (_nestingLimit == 0) return JsonError::TooDeep;

  // Check opening brace
  if (!eat('{')) return JsonError::OpeningBraceExpected;
  if (eat('}')) return JsonError::Ok;

  // Read each key value pair
  for (;;) {
    // 1 - Parse key
    const char *key = parseString();
    if (!key) return JsonError::NoMemory;
    if (!eat(':')) return JsonError::ColonExpected;

    // 2 - Parse value
    JsonVariant value;
    _nestingLimit--;
    JsonError error = parse(value);
    _nestingLimit++;
    if (error != JsonError::Ok) return error;
    if (!object.set(key, value)) return JsonError::NoMemory;

    // 3 - More keys/values?
    if (eat('}')) return JsonError::Ok;
    if (!eat(',')) return JsonError::ClosingBraceExpected;
  }
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonError
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parse(
    JsonVariant &variant) {
  skipSpacesAndComments(_reader);

  switch (_reader.current()) {
    case '[':
      return parseArray(variant);

    case '{':
      return parseObject(variant);

    default:
      return parseValue(variant);
  }
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonError
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseArray(
    JsonVariant &variant) {
  JsonArray *array = new (_buffer) JsonArray(_buffer);
  if (!array) return JsonError::NoMemory;
  variant = array;
  return parse(*array);
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonError
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseObject(
    JsonVariant &variant) {
  JsonObject *object = new (_buffer) JsonObject(_buffer);
  if (!object) return JsonError::NoMemory;
  variant = object;
  return parse(*object);
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonError
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseValue(
    JsonVariant &variant) {
  bool hasQuotes = isQuote(_reader.current());
  const char *value = parseString();
  if (value == NULL) return JsonError::NoMemory;
  if (hasQuotes) {
    variant = value;
  } else {
    variant = RawJson(value);
  }
  return JsonError::Ok;
}

template <typename TReader, typename TWriter>
inline const char *
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseString() {
  typename RemoveReference<TWriter>::type::String str = _writer.startString();

  skipSpacesAndComments(_reader);
  char c = _reader.current();

  if (isQuote(c)) {  // quotes
    _reader.move();
    char stopChar = c;
    for (;;) {
      c = _reader.current();
      if (c == '\0') break;
      _reader.move();

      if (c == stopChar) break;

      if (c == '\\') {
        // replace char
        c = Encoding::unescapeChar(_reader.current());
        if (c == '\0') break;
        _reader.move();
      }

      str.append(c);
    }
  } else {  // no quotes
    for (;;) {
      if (!canBeInNonQuotedString(c)) break;
      _reader.move();
      str.append(c);
      c = _reader.current();
    }
  }

  return str.c_str();
}
