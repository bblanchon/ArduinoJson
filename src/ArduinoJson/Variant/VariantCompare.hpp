// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Numbers/arithmeticCompare.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Polyfills/utility.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/JsonVariantVisitor.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct ComparerBase : JsonVariantVisitor<CompareResult> {};

template <typename T, typename Enable = void>
struct Comparer;

template <typename T>
struct Comparer<T, enable_if_t<IsString<T>::value>> : ComparerBase {
  T rhs;  // TODO: store adapted string?

  explicit Comparer(T value) : rhs(value) {}

  CompareResult visit(JsonString lhs) {
    int i = stringCompare(adaptString(rhs), adaptString(lhs));
    if (i < 0)
      return COMPARE_RESULT_GREATER;
    else if (i > 0)
      return COMPARE_RESULT_LESS;
    else
      return COMPARE_RESULT_EQUAL;
  }

  CompareResult visit(nullptr_t) {
    if (adaptString(rhs).isNull())
      return COMPARE_RESULT_EQUAL;
    else
      return COMPARE_RESULT_DIFFER;
  }

  using ComparerBase::visit;
};

template <typename T>
struct Comparer<
    T, enable_if_t<is_integral<T>::value || is_floating_point<T>::value>>
    : ComparerBase {
  T rhs;

  explicit Comparer(T value) : rhs(value) {}

  template <typename U>
  enable_if_t<is_floating_point<U>::value || is_integral<U>::value,
              CompareResult>
  visit(const U& lhs) {
    return arithmeticCompare(lhs, rhs);
  }

  template <typename U>
  enable_if_t<!is_floating_point<U>::value && !is_integral<U>::value,
              CompareResult>
  visit(const U& lhs) {
    return ComparerBase::visit(lhs);
  }
};

struct NullComparer : ComparerBase {
  CompareResult visit(nullptr_t) {
    return COMPARE_RESULT_EQUAL;
  }

  using ComparerBase::visit;
};

template <>
struct Comparer<nullptr_t, void> : NullComparer {
  explicit Comparer(nullptr_t) : NullComparer() {}
};

struct ArrayComparer : ComparerBase {
  JsonArrayConst rhs_;

  explicit ArrayComparer(JsonArrayConst rhs) : rhs_(rhs) {}

  CompareResult visit(JsonArrayConst lhs) {
    if (rhs_ == lhs)
      return COMPARE_RESULT_EQUAL;
    else
      return COMPARE_RESULT_DIFFER;
  }

  using ComparerBase::visit;
};

struct ObjectComparer : ComparerBase {
  JsonObjectConst rhs_;

  explicit ObjectComparer(JsonObjectConst rhs) : rhs_(rhs) {}

  CompareResult visit(JsonObjectConst lhs) {
    if (lhs == rhs_)
      return COMPARE_RESULT_EQUAL;
    else
      return COMPARE_RESULT_DIFFER;
  }

  using ComparerBase::visit;
};

struct RawComparer : ComparerBase {
  RawString rhs_;

  explicit RawComparer(RawString rhs) : rhs_(rhs) {}

  CompareResult visit(RawString lhs) {
    size_t size = rhs_.size() < lhs.size() ? rhs_.size() : lhs.size();
    int n = memcmp(lhs.data(), rhs_.data(), size);
    if (n < 0)
      return COMPARE_RESULT_LESS;
    else if (n > 0)
      return COMPARE_RESULT_GREATER;
    else
      return COMPARE_RESULT_EQUAL;
  }

  using ComparerBase::visit;
};

struct VariantComparer : ComparerBase {
  JsonVariantConst rhs;

  explicit VariantComparer(JsonVariantConst value) : rhs(value) {}

  CompareResult visit(JsonArrayConst lhs) {
    ArrayComparer comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visit(JsonObjectConst lhs) {
    ObjectComparer comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visit(JsonFloat lhs) {
    Comparer<JsonFloat> comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visit(JsonString lhs) {
    Comparer<JsonString> comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visit(RawString value) {
    RawComparer comparer(value);
    return reverseResult(comparer);
  }

  CompareResult visit(JsonInteger lhs) {
    Comparer<JsonInteger> comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visit(JsonUInt lhs) {
    Comparer<JsonUInt> comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visit(bool lhs) {
    Comparer<bool> comparer(lhs);
    return reverseResult(comparer);
  }

  CompareResult visit(nullptr_t) {
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
struct Comparer<
    T, enable_if_t<is_convertible<T, ArduinoJson::JsonVariantConst>::value>>
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
