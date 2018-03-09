// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Data/Encoding.hpp"
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
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseAnythingTo(
    JsonVariant *destination) {
  if (_nestingLimit == 0) return JsonError::TooDeep;
  _nestingLimit--;
  JsonError error = parseAnythingToUnsafe(destination);
  _nestingLimit++;
  return error;
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonError
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseAnythingToUnsafe(
    JsonVariant *destination) {
  skipSpacesAndComments(_reader);

  switch (_reader.current()) {
    case '[':
      return parseArrayTo(destination);

    case '{':
      return parseObjectTo(destination);

    default:
      return parseStringTo(destination);
  }
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonError
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parse(JsonArray &array) {
  // Check opening braket
  if (!eat('[')) return JsonError::OpeningBracketExpected;
  if (eat(']')) return JsonError::Ok;

  // Read each value
  for (;;) {
    // 1 - Parse value
    JsonVariant value;
    JsonError error = parseAnythingTo(&value);
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
    JsonError error = parseAnythingTo(&value);
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
  return parseAnythingTo(&variant);
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonError
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseArrayTo(
    JsonVariant *destination) {
  JsonArray *array = new (_buffer) JsonArray(_buffer);
  if (!array) return JsonError::NoMemory;
  *destination = array;
  return parse(*array);
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonError
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseObjectTo(
    JsonVariant *destination) {
  JsonObject *object = new (_buffer) JsonObject(_buffer);
  if (!object) return JsonError::NoMemory;
  *destination = object;
  return parse(*object);
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

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonError
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseStringTo(
    JsonVariant *destination) {
  bool hasQuotes = isQuote(_reader.current());
  const char *value = parseString();
  if (value == NULL) return JsonError::NoMemory;
  if (hasQuotes) {
    *destination = value;
  } else {
    *destination = RawJson(value);
  }
  return JsonError::Ok;
}
