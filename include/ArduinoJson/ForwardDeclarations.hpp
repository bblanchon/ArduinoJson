// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {
class JsonArray;
class JsonBuffer;
class JsonObject;
class JsonObjectIterator;
class JsonObjectKeyValue;
class JsonValue;
template <typename T>
class JsonIterator;
template <typename T>
class JsonConstIterator;

namespace Internals {
class IndentedPrint;
class JsonNode;
class JsonParser;
class JsonWriter;
}
}
