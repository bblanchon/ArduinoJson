// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include "ArduinoJson/Configuration.hpp"

// Include Arduino.h before stdlib.h to avoid conflict with atexit()
// https://github.com/bblanchon/ArduinoJson/pull/1693#issuecomment-1001060240
#if ARDUINOJSON_ENABLE_ARDUINO_STRING || ARDUINOJSON_ENABLE_ARDUINO_STREAM || \
    ARDUINOJSON_ENABLE_ARDUINO_PRINT || ARDUINOJSON_ENABLE_PROGMEM
#  include <Arduino.h>
#endif

#if !ARDUINOJSON_DEBUG
#  ifdef __clang__
#    pragma clang system_header
#  elif defined __GNUC__
#    pragma GCC system_header
#  endif
#endif

#include "ArduinoJson/Array/JsonArray.hpp"
#include "ArduinoJson/Object/JsonObject.hpp"
#include "ArduinoJson/Variant/JsonVariantConst.hpp"

#include "ArduinoJson/Document/DynamicJsonDocument.hpp"
#include "ArduinoJson/Document/StaticJsonDocument.hpp"

#include "ArduinoJson/Array/ElementProxy.hpp"
#include "ArduinoJson/Array/JsonArrayImpl.hpp"
#include "ArduinoJson/Array/Utilities.hpp"
#include "ArduinoJson/Collection/CollectionImpl.hpp"
#include "ArduinoJson/Object/JsonObjectImpl.hpp"
#include "ArduinoJson/Object/MemberProxy.hpp"
#include "ArduinoJson/Variant/ConverterImpl.hpp"
#include "ArduinoJson/Variant/VariantCompare.hpp"
#include "ArduinoJson/Variant/VariantImpl.hpp"

#include "ArduinoJson/Json/JsonDeserializer.hpp"
#include "ArduinoJson/Json/JsonSerializer.hpp"
#include "ArduinoJson/Json/PrettyJsonSerializer.hpp"
#include "ArduinoJson/MsgPack/MsgPackDeserializer.hpp"
#include "ArduinoJson/MsgPack/MsgPackSerializer.hpp"

#include "ArduinoJson/compatibility.hpp"

namespace ArduinoJson {
using ARDUINOJSON_NAMESPACE::BasicJsonDocument;
using ARDUINOJSON_NAMESPACE::copyArray;
using ARDUINOJSON_NAMESPACE::DeserializationError;
using ARDUINOJSON_NAMESPACE::deserializeJson;
using ARDUINOJSON_NAMESPACE::deserializeMsgPack;
using ARDUINOJSON_NAMESPACE::DynamicJsonDocument;
using ARDUINOJSON_NAMESPACE::JsonArray;
using ARDUINOJSON_NAMESPACE::JsonArrayConst;
using ARDUINOJSON_NAMESPACE::JsonDocument;
using ARDUINOJSON_NAMESPACE::JsonFloat;
using ARDUINOJSON_NAMESPACE::JsonInteger;
using ARDUINOJSON_NAMESPACE::JsonObject;
using ARDUINOJSON_NAMESPACE::JsonObjectConst;
using ARDUINOJSON_NAMESPACE::JsonPair;
using ARDUINOJSON_NAMESPACE::JsonPairConst;
using ARDUINOJSON_NAMESPACE::JsonString;
using ARDUINOJSON_NAMESPACE::JsonUInt;
using ARDUINOJSON_NAMESPACE::JsonVariant;
using ARDUINOJSON_NAMESPACE::JsonVariantConst;
using ARDUINOJSON_NAMESPACE::measureJson;
using ARDUINOJSON_NAMESPACE::serialized;
using ARDUINOJSON_NAMESPACE::serializeJson;
using ARDUINOJSON_NAMESPACE::serializeJsonPretty;
using ARDUINOJSON_NAMESPACE::serializeMsgPack;
using ARDUINOJSON_NAMESPACE::StaticJsonDocument;

namespace DeserializationOption {
using ARDUINOJSON_NAMESPACE::Filter;
using ARDUINOJSON_NAMESPACE::NestingLimit;
}  // namespace DeserializationOption
}  // namespace ArduinoJson
