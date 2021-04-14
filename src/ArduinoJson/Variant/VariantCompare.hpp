// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Misc/Visitable.hpp>
#include <ArduinoJson/Numbers/arithmeticCompare.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/IsString.hpp>
#include <ArduinoJson/Variant/Visitor.hpp>

namespace ARDUINOJSON_NAMESPACE {

class CollectionData;

struct ComparerBase : Visitor<CompareResult> {};

template <typename T, typename Enable = void>
struct Comparer;

template <typename T>
struct Comparer<T, typename enable_if<IsString<T>::value>::type>
    : ComparerBase {
  T rhs;

  explicit Comparer(T value) : rhs(value) {}

  CompareResult visitString(const char *lhs) {
    int i = adaptString(rhs).compare(lhs);
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

  CompareResult visitFloat(Float lhs) {
    return arithmeticCompare(lhs, rhs);
  }

  CompareResult visitSignedInteger(Integer lhs) {
    return arithmeticCompare(lhs, rhs);
  }

  CompareResult visitUnsignedInteger(UInt lhs) {
    return arithmeticCompare(lhs, rhs);
  }

  CompareResult visitBoolean(bool lhs) {
    return visitUnsignedInteger(static_cast<UInt>(lhs));
  }
};

struct NullComparer : ComparerBase {
  CompareResult visitNull() {
    return COMPARE_RESULT_EQUAL;
  }
};

#if ARDUINOJSON_HAS_NULLPTR
template <>
struct Comparer<decltype(nullptr), void> : NullComparer {
  explicit Comparer(decltype(nullptr)) : NullComparer() {}
};
#endif

struct ArrayComparer : ComparerBase {
  const CollectionData *_rhs;

  explicit ArrayComparer(const CollectionData &rhs) : _rhs(&rhs) {}

  CompareResult visitArray(const CollectionData &lhs) {
    if (lhs.equalsArray(*_rhs))
      return COMPARE_RESULT_EQUAL;
    else
      return COMPARE_RESULT_DIFFER;
  }
};

struct ObjectComparer : ComparerBase {
  const CollectionData *_rhs;

  explicit ObjectComparer(const CollectionData &rhs) : _rhs(&rhs) {}

  CompareResult visitObject(const CollectionData &lhs) {
    if (lhs.equalsObject(*_rhs))
      return COMPARE_RESULT_EQUAL;
    else
      return COMPARE_RESULT_DIFFER;
  }
};

struct RawComparer : ComparerBase {
  const char *_rhsData;
  size_t _rhsSize;

  explicit RawComparer(const char *rhsData, size_t rhsSize)
      : _rhsData(rhsData), _rhsSize(rhsSize) {}

  CompareResult visitRawJson(const char *lhsData, size_t lhsSize) {
    size_t size = _rhsSize < lhsSize ? _rhsSize : lhsSize;
    int n = memcmp(lhsData, _rhsData, size);
    if (n < 0)
      return COMPARE_RESULT_LESS;
    else if (n > 0)
      return COMPARE_RESULT_GREATER;
    else
      return COMPARE_RESULT_EQUAL;
  }
};

template <typename T>
struct Comparer<T, typename enable_if<IsVisitable<T>::value>::type>
    : ComparerBase {
  T rhs;

  explicit Comparer(T value) : rhs(value) {}

  CompareResult visitArray(const CollectionData &lhs) {
    ArrayComparer comparer(lhs);
    return accept(comparer);
  }

  CompareResult visitObject(const CollectionData &lhs) {
    ObjectComparer comparer(lhs);
    return accept(comparer);
  }

  CompareResult visitFloat(Float lhs) {
    Comparer<Float> comparer(lhs);
    return accept(comparer);
  }

  CompareResult visitString(const char *lhs) {
    Comparer<const char *> comparer(lhs);
    return accept(comparer);
  }

  CompareResult visitRawJson(const char *lhsData, size_t lhsSize) {
    RawComparer comparer(lhsData, lhsSize);
    return accept(comparer);
  }

  CompareResult visitSignedInteger(Integer lhs) {
    Comparer<Integer> comparer(lhs);
    return accept(comparer);
  }

  CompareResult visitUnsignedInteger(UInt lhs) {
    Comparer<UInt> comparer(lhs);
    return accept(comparer);
  }

  CompareResult visitBoolean(bool lhs) {
    Comparer<bool> comparer(lhs);
    return accept(comparer);
  }

  CompareResult visitNull() {
    NullComparer comparer;
    return accept(comparer);
  }

 private:
  template <typename TComparer>
  CompareResult accept(TComparer &comparer) {
    CompareResult reversedResult = rhs.accept(comparer);
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

template <typename T1, typename T2>
CompareResult compare(const T1 &lhs, const T2 &rhs) {
  Comparer<T2> comparer(rhs);
  return lhs.accept(comparer);
}

inline int variantCompare(const VariantData *a, const VariantData *b) {
  return compare(VariantConstRef(a), VariantConstRef(b));
}

}  // namespace ARDUINOJSON_NAMESPACE
