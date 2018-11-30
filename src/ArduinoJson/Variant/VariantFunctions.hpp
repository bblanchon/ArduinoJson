// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Array/ArrayFunctions.hpp"
#include "../Misc/SerializedValue.hpp"
#include "../Numbers/parseFloat.hpp"
#include "../Numbers/parseInteger.hpp"
#include "../Object/ObjectFunctions.hpp"
#include "VariantData.hpp"

namespace ARDUINOJSON_NAMESPACE {
template <typename T>
inline T variantAsIntegral(const VariantData* var) {
  if (!var) return 0;
  switch (var->type) {
    case JSON_POSITIVE_INTEGER:
    case JSON_BOOLEAN:
      return T(var->content.asInteger);
    case JSON_NEGATIVE_INTEGER:
      return T(~var->content.asInteger + 1);
    case JSON_LINKED_STRING:
    case JSON_OWNED_STRING:
      return parseInteger<T>(var->content.asString);
    case JSON_FLOAT:
      return T(var->content.asFloat);
    default:
      return 0;
  }
}

inline bool variantAsBoolean(const VariantData* var) {
  return variantAsIntegral<int>(var) != 0;
}

// T = float/double
template <typename T>
inline T variantAsFloat(const VariantData* var) {
  if (!var) return 0;
  switch (var->type) {
    case JSON_POSITIVE_INTEGER:
    case JSON_BOOLEAN:
      return static_cast<T>(var->content.asInteger);
    case JSON_NEGATIVE_INTEGER:
      return -static_cast<T>(var->content.asInteger);
    case JSON_LINKED_STRING:
    case JSON_OWNED_STRING:
      return parseFloat<T>(var->content.asString);
    case JSON_FLOAT:
      return static_cast<T>(var->content.asFloat);
    default:
      return 0;
  }
}

inline const char* variantAsString(const VariantData* var) {
  if (!var) return 0;
  switch (var->type) {
    case JSON_LINKED_STRING:
    case JSON_OWNED_STRING:
      return var->content.asString;
    default:
      return 0;
  }
}

inline ArrayData* variantAsArray(VariantData* var) {
  if (var && var->type == JSON_ARRAY)
    return &var->content.asArray;
  else
    return 0;
}

inline const ArrayData* variantAsArray(const VariantData* var) {
  if (var && var->type == JSON_ARRAY)
    return &var->content.asArray;
  else
    return 0;
}

inline ObjectData* variantAsObject(VariantData* var) {
  if (var && var->type == JSON_OBJECT)
    return &var->content.asObject;
  else
    return 0;
}

inline const ObjectData* variantAsObject(const VariantData* var) {
  if (var && var->type == JSON_OBJECT)
    return &var->content.asObject;
  else
    return 0;
}

inline bool variantSetBoolean(VariantData* var, bool value) {
  if (!var) return false;
  var->type = JSON_BOOLEAN;
  var->content.asInteger = static_cast<UInt>(value);
  return true;
}

inline bool variantSetFloat(VariantData* var, Float value) {
  if (!var) return false;
  var->type = JSON_FLOAT;
  var->content.asFloat = value;
  return true;
}

template <typename T>
inline bool variantSetSignedInteger(VariantData* var, T value) {
  if (!var) return false;
  if (value >= 0) {
    var->type = JSON_POSITIVE_INTEGER;
    var->content.asInteger = static_cast<UInt>(value);
  } else {
    var->type = JSON_NEGATIVE_INTEGER;
    var->content.asInteger = ~static_cast<UInt>(value) + 1;
  }
  return true;
}

inline bool variantSetUnsignedInteger(VariantData* var, UInt value) {
  if (!var) return false;
  var->type = JSON_POSITIVE_INTEGER;
  var->content.asInteger = static_cast<UInt>(value);
  return true;
}

inline bool variantSetLinkedRaw(VariantData* var,
                                SerializedValue<const char*> value) {
  if (!var) return false;
  var->type = JSON_LINKED_RAW;
  var->content.asRaw.data = value.data();
  var->content.asRaw.size = value.size();
  return true;
}

template <typename T>
inline bool variantSetOwnedRaw(VariantData* var, SerializedValue<T> value,
                               MemoryPool* pool) {
  if (!var) return false;
  char* dup = makeString(value.data(), value.size()).save(pool);
  if (dup) {
    var->type = JSON_OWNED_RAW;
    var->content.asRaw.data = dup;
    var->content.asRaw.size = value.size();
    return true;
  } else {
    var->type = JSON_NULL;
    return false;
  }
}

template <typename T>
inline bool variantSetString(VariantData* var, T value, MemoryPool* pool) {
  if (!var) return false;
  char* dup = value.save(pool);
  if (dup) {
    var->type = JSON_OWNED_STRING;
    var->content.asString = dup;
    return true;
  } else {
    var->type = JSON_NULL;
    return false;
  }
}

inline bool variantSetOwnedString(VariantData* var, char* s) {
  if (!var) return false;
  var->type = JSON_OWNED_STRING;
  var->content.asString = s;
  return true;
}

inline bool variantSetString(VariantData* var, const char* value) {
  if (!var) return false;
  var->type = JSON_LINKED_STRING;
  var->content.asString = value;
  return true;
}

inline void variantSetNull(VariantData* var) {
  if (!var) return;
  var->type = JSON_NULL;
}

inline ArrayData* variantToArray(VariantData* var) {
  if (!var) return 0;
  var->type = JSON_ARRAY;
  var->content.asArray.head = 0;
  var->content.asArray.tail = 0;
  return &var->content.asArray;
}

inline ObjectData* variantToObject(VariantData* var) {
  if (!var) return 0;
  var->type = JSON_OBJECT;
  var->content.asObject.head = 0;
  var->content.asObject.tail = 0;
  return &var->content.asObject;
}

inline bool variantCopy(VariantData* dst, const VariantData* src,
                        MemoryPool* pool) {
  if (!dst) return false;
  if (!src) {
    dst->type = JSON_NULL;
    return true;
  }
  switch (src->type) {
    case JSON_ARRAY:
      return arrayCopy(variantToArray(dst), &src->content.asArray, pool);
    case JSON_OBJECT:
      return objectCopy(variantToObject(dst), &src->content.asObject, pool);
    case JSON_OWNED_STRING:
      return variantSetString(
          dst, ZeroTerminatedRamString(src->content.asString), pool);
    case JSON_OWNED_RAW:
      return variantSetOwnedRaw(
          dst, serialized(src->content.asRaw.data, src->content.asRaw.size),
          pool);
    default:
      // caution: don't override keyIsOwned
      dst->type = src->type;
      dst->content = src->content;
      return true;
  }
}

inline bool variantIsInteger(const VariantData* var) {
  return var && (var->type == JSON_POSITIVE_INTEGER ||
                 var->type == JSON_NEGATIVE_INTEGER);
}

inline bool variantIsFloat(const VariantData* var) {
  return var &&
         (var->type == JSON_FLOAT || var->type == JSON_POSITIVE_INTEGER ||
          var->type == JSON_NEGATIVE_INTEGER);
}

inline bool variantIsString(const VariantData* var) {
  return var &&
         (var->type == JSON_LINKED_STRING || var->type == JSON_OWNED_STRING);
}

inline bool variantIsArray(const VariantData* var) {
  return var && var->type == JSON_ARRAY;
}

inline bool variantIsObject(const VariantData* var) {
  return var && var->type == JSON_OBJECT;
}

inline bool variantIsNull(const VariantData* var) {
  return var == 0 || var->type == JSON_NULL;
}

inline bool variantEquals(const VariantData* a, const VariantData* b) {
  if (a == b) return true;
  if (!a || !b) return false;
  if (a->type != b->type) return false;

  switch (a->type) {
    case JSON_LINKED_STRING:
    case JSON_OWNED_STRING:
      return !strcmp(a->content.asString, b->content.asString);

    case JSON_LINKED_RAW:
    case JSON_OWNED_RAW:
      return a->content.asRaw.size == b->content.asRaw.size &&
             !memcmp(a->content.asRaw.data, b->content.asRaw.data,
                     a->content.asRaw.size);

    case JSON_BOOLEAN:
    case JSON_POSITIVE_INTEGER:
    case JSON_NEGATIVE_INTEGER:
      return a->content.asInteger == b->content.asInteger;

    case JSON_ARRAY:
      return arrayEquals(&a->content.asArray, &b->content.asArray);

    case JSON_OBJECT:
      return objectEquals(&a->content.asObject, &b->content.asObject);

    case JSON_FLOAT:
      return a->content.asFloat == b->content.asFloat;

    case JSON_NULL:
    default:
      return true;
  }
}
}  // namespace ARDUINOJSON_NAMESPACE
