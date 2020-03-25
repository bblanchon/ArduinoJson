// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include "ArduinoJson/Configuration.hpp"

#if !ARDUINOJSON_DEBUG
#ifdef __clang__
#pragma clang system_header
#elif defined __GNUC__
#pragma GCC system_header
#endif
#endif

#include "ArduinoJson/Array/ArrayRef.hpp"
#include "ArduinoJson/Object/ObjectRef.hpp"
#include "ArduinoJson/Variant/VariantRef.hpp"

#include "ArduinoJson/Document/DynamicJsonDocument.hpp"
#include "ArduinoJson/Document/StaticJsonDocument.hpp"

#include "ArduinoJson/Array/ArrayImpl.hpp"
#include "ArduinoJson/Array/ElementProxy.hpp"
#include "ArduinoJson/Array/Utilities.hpp"
#include "ArduinoJson/Collection/CollectionImpl.hpp"
#include "ArduinoJson/Object/MemberProxy.hpp"
#include "ArduinoJson/Object/ObjectImpl.hpp"
#include "ArduinoJson/Variant/VariantAsImpl.hpp"
#include "ArduinoJson/Variant/VariantImpl.hpp"

#include "ArduinoJson/Json/JsonDeserializer.hpp"
#include "ArduinoJson/Json/JsonSerializer.hpp"
#include "ArduinoJson/Json/PrettyJsonSerializer.hpp"
#include "ArduinoJson/MsgPack/MsgPackDeserializer.hpp"
#include "ArduinoJson/MsgPack/MsgPackSerializer.hpp"

#include "ArduinoJson/compatibility.hpp"

namespace ArduinoJson {
typedef ARDUINOJSON_NAMESPACE::ArrayConstRef JsonArrayConst;
typedef ARDUINOJSON_NAMESPACE::ArrayRef JsonArray;
typedef ARDUINOJSON_NAMESPACE::Float JsonFloat;
typedef ARDUINOJSON_NAMESPACE::Integer JsonInteger;
typedef ARDUINOJSON_NAMESPACE::ObjectConstRef JsonObjectConst;
typedef ARDUINOJSON_NAMESPACE::ObjectRef JsonObject;
typedef ARDUINOJSON_NAMESPACE::Pair JsonPair;
typedef ARDUINOJSON_NAMESPACE::PairConst JsonPairConst;
typedef ARDUINOJSON_NAMESPACE::String JsonString;
typedef ARDUINOJSON_NAMESPACE::UInt JsonUInt;
typedef ARDUINOJSON_NAMESPACE::VariantConstRef JsonVariantConst;
typedef ARDUINOJSON_NAMESPACE::VariantRef JsonVariant;
using ARDUINOJSON_NAMESPACE::BasicJsonDocument;
using ARDUINOJSON_NAMESPACE::copyArray;
using ARDUINOJSON_NAMESPACE::DeserializationError;
using ARDUINOJSON_NAMESPACE::deserializeJson;
using ARDUINOJSON_NAMESPACE::deserializeMsgPack;
using ARDUINOJSON_NAMESPACE::DynamicJsonDocument;
using ARDUINOJSON_NAMESPACE::JsonDocument;
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
