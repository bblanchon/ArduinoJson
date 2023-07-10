// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Numbers/arithmeticCompare.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/JsonVariantVisitor.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct ComparerBase : JsonVariantVisitor<CompareResult> {};

template <typename T, typename Enable = void>
struct Comparer;

template <typename T>
struct Comparer<T, typename enable_if<IsString<T>::value>::type>
    : ComparerBase {
  T rhs;  // TODO: store adapted string?

  explicit Comparer(T value) : rhs(value) {}

  CompareResult visitString(JsonString lhs) {
    int i = stringCompare(adaptString(rhs), adaptString(lhs));
    if (i < 0)
      return COMPARE_RESULT_GREATER;
    else if (i > 0)
      return COMPARE_RESULT_LESS;
    else
      return COMPARE_RESULT_EQUAL;
  }

  CompareResult visitNull() {
    if (adaptString(rhs).isNull())
      return COMPARE_RESULT_EQUAL;
    else
      return COMPARE_RESULT_DIFFER;
  }
};

template <typename T>
struct Comparer<T, typename enable_if<is_integral<T>::value ||
                                      is_floating_point<T>::value>::type>
    : ComparerBase {
  T rhs;

  explicit Comparer(T value) : rhs(value) {}

  CompareResult visitFloat(JsonFloat lhs) {
    return arithmeticCompare(lhs, rhs);
  }

  CompareResult visitSignedInteger(JsonInteger lhs) {
    return arithmeticCompare(lhs, rhs);
  }

  CompareResult visitUnsignedInteger(JsonUInt lhs) {
    return arithmeticCompare(lhs, rhs);
  }

  CompareResult visitBoolean(bool lhs) {
    return visitUnsignedInteger(static_cast<JsonUInt>(lhs));
  }
};

struct NullComparer : ComparerBase {
  CompareResult visitNull() {
    return COMPARE_RESULT_EQUAL;
  }
};

template <>
struct Comparer<decltype(nullptr), void> : NullComparer {
  explicit Comparer(decltype(nullptr)) : NullComparer() {}
};

struct ArrayComparer : ComparerBase {
  JsonArrayConst rhs_;

  explicit ArrayComparer(JsonArrayConst rhs) : rhs_(rhs) {}

  CompareResult visitArray(JsonArrayConst lhs) {
    if (rhs_ == lhs)
      return COMPARE_RESULT_EQUAL;
    else
      return COMPARE_RESULT_DIFFER;
  }
};

struct ObjectComparer : ComparerBase {
  JsonObjectConst rhs_;

  explicit ObjectComparer(JsonObjectConst rhs) : rhs_(rhs) {}

  CompareResult visitObject(JsonObjectConst lhs) {
    if (lhs == rhs_)
      return COMPARE_RESULT_EQUAL;
    else
      return COMPARE_RESULT_DIFFER;
  }
};

struct RawComparer : ComparerBase {
  RawString rhs_;

  explicit RawComparer(RawString rhs) : rhs_(rhs) {}

  CompareResult visitRawString(RawString lhs) {
    size_t size = rhs_.size() < lhs.size() ? rhs_.size() : lhs.size();
    int n = memcmp(lhs.data(), rhs_.data(), size);
    if (n < 0)
      return COMPARE_RESULT_LESS;
    else if (n > 0)
      return COMPARE_RESULT_GREATER;
    else
      return COMPARE_RESULT_EQUAL;
  }
};

struct VariantComparer : ComparerBase {
  JsonVariantConst rhs;

  explicit VariantComparer(JsonVariantConst value) : rhs(value) {}

  CompareResult visitArray(JsonArrayConst lhs) {
    ArrayComparer comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visitObject(JsonObjectConst lhs) {
    ObjectComparer comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visitFloat(JsonFloat lhs) {
    Comparer<JsonFloat> comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visitString(JsonString lhs) {
    Comparer<JsonString> comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visitRawString(RawString value) {
    RawComparer comparer(value);
    return reverseResult(comparer);
  }

  CompareResult visitSignedInteger(JsonInteger lhs) {
    Comparer<JsonInteger> comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visitUnsignedInteger(JsonUInt lhs) {
    Comparer<JsonUInt> comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visitBoolean(bool lhs) {
    Comparer<bool> comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visitNull() {
    NullComparer comparer;
    return reverseResult(comparer);
  }

 private:
  template <typename TComparer>
  CompareResult reverseResult(TComparer& comparer) {
    CompareResult reversedResult = accept(rhs, comparer);
    switch (reversedResult) {
      case COMPARE_RESULT_GREATER:
        return COMPARE_RESULT_LESS;
      case COMPARE_RESULT_LESS:
        return COMPARE_RESULT_GREATER;
      default:
        return reversedResult;
    }
  }
};

template <typename T>
struct Comparer<T, typename enable_if<is_convertible<
                       T, ArduinoJson::JsonVariantConst>::value>::type>
    : VariantComparer {
  explicit Comparer(const T& value)
      : VariantComparer(static_cast<JsonVariantConst>(value)) {}
};

template <typename T>
CompareResult compare(ArduinoJson::JsonVariantConst lhs, const T& rhs) {
  Comparer<T> comparer(rhs);
  return accept(lhs, comparer);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
