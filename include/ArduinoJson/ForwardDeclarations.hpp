// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {
class JsonArray;
class JsonArrayConstIterator;
class JsonArrayIterator;
class JsonBuffer;
class JsonObject;
class JsonObjectConstIterator;
class JsonObjectIterator;
struct JsonPair;
class JsonValue;

namespace Internals {
class IndentedPrint;
class JsonArrayNode;
class JsonObjectNode;
class JsonParser;
class JsonWriter;
}
}
