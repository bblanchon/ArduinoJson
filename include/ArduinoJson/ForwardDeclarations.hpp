// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

// TODO: cleanup

namespace ArduinoJson {
class JsonArray;
class JsonBuffer;
class JsonPair;
class JsonValue;
class JsonObject;

namespace Internals {
class IndentedPrint;
class JsonArrayConstIterator;
class JsonArrayImpl;
class JsonArrayIterator;
class JsonArrayNode;
class JsonObjectConstIterator;
class JsonObjectImpl;
class JsonObjectIterator;
class JsonObjectNode;
class JsonParser;
class JsonValueImpl;
class JsonWriter;
}
}
