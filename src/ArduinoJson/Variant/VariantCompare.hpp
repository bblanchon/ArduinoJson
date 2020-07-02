// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Misc/Visitable.hpp>
#include <ArduinoJson/Numbers/arithmeticCompare.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/IsString.hpp>

namespace ARDUINOJSON_NAMESPACE {

class CollectionData;

struct ComparerBase {
  CompareResult result;

  ComparerBase() : result(COMPARE_RESULT_DIFFER) {}

  void visitArray(const CollectionData &) {}
  void visitBoolean(bool) {}
  void visitFloat(Float) {}
  void visitNegativeInteger(UInt) {}
  void visitNull() {}
  void visitObject(const CollectionData &) {}
  void visitPositiveInteger(UInt) {}
  void visitRawJson(const char *, size_t) {}
  void visitString(const char *) {}
};

template <typename T, typename Enable = void>
struct Comparer;

template <typename T>
struct Comparer<T, typename enable_if<IsString<T>::value>::type>
    : ComparerBase {
  T rhs;

  explicit Comparer(T value) : rhs(value) {}

  void visitString(const char *lhs) {
    int i = adaptString(rhs).compare(lhs);
    if (i < 0)
      result = COMPARE_RESULT_GREATER;
    else if (i > 0)
      result = COMPARE_RESULT_LESS;
    else
      result = COMPARE_RESULT_EQUAL;
  }

  void visitNull() {
    if (adaptString(rhs).isNull())
      result = COMPARE_RESULT_EQUAL;
  }
};

template <typename T>
struct Comparer<T, typename enable_if<is_integral<T>::value ||
                                      is_floating_point<T>::value>::type>
    : ComparerBase {
  T rhs;

  explicit Comparer(T value) : rhs(value) {}

  void visitFloat(Float lhs) {
    result = arithmeticCompare(lhs, rhs);
  }

  void visitNegativeInteger(UInt lhs) {
    result = arithmeticCompareNegateLeft(lhs, rhs);
  }

  void visitPositiveInteger(UInt lhs) {
    result = arithmeticCompare(lhs, rhs);
  }

  void visitBoolean(bool lhs) {
    visitPositiveInteger(static_cast<UInt>(lhs));
  }
};

struct NullComparer : ComparerBase {
  void visitNull() {
    result = COMPARE_RESULT_EQUAL;
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

  void visitArray(const CollectionData &lhs) {
    if (lhs.equalsArray(*_rhs))
      result = COMPARE_RESULT_EQUAL;
  }
};

struct NegativeIntegerComparer : ComparerBase {
  UInt _rhs;

  explicit NegativeIntegerComparer(UInt rhs) : _rhs(rhs) {}

  void visitFloat(Float lhs) {
    result = arithmeticCompareNegateRight(lhs, _rhs);
  }

  void visitNegativeInteger(UInt lhs) {
    result = arithmeticCompare(_rhs, lhs);
  }

  void visitPositiveInteger(UInt) {
    result = COMPARE_RESULT_GREATER;
  }

  void visitBoolean(bool) {
    result = COMPARE_RESULT_GREATER;
  }
};

struct ObjectComparer : ComparerBase {
  const CollectionData *_rhs;

  explicit ObjectComparer(const CollectionData &rhs) : _rhs(&rhs) {}

  void visitObject(const CollectionData &lhs) {
    if (lhs.equalsObject(*_rhs))
      result = COMPARE_RESULT_EQUAL;
  }
};

struct RawComparer : ComparerBase {
  const char *_rhsData;
  size_t _rhsSize;

  explicit RawComparer(const char *rhsData, size_t rhsSize)
      : _rhsData(rhsData), _rhsSize(rhsSize) {}

  void visitRawJson(const char *lhsData, size_t lhsSize) {
    size_t size = _rhsSize < lhsSize ? _rhsSize : lhsSize;
    int n = memcmp(lhsData, _rhsData, size);
    if (n < 0)
      result = COMPARE_RESULT_LESS;
    else if (n > 0)
      result = COMPARE_RESULT_GREATER;
    else
      result = COMPARE_RESULT_EQUAL;
  }
};

template <typename T>
struct Comparer<T, typename enable_if<IsVisitable<T>::value>::type>
    : ComparerBase {
  T rhs;

  explicit Comparer(T value) : rhs(value) {}

  void visitArray(const CollectionData &lhs) {
    ArrayComparer comparer(lhs);
    accept(comparer);
  }

  void visitObject(const CollectionData &lhs) {
    ObjectComparer comparer(lhs);
    accept(comparer);
  }

  void visitFloat(Float lhs) {
    Comparer<Float> comparer(lhs);
    accept(comparer);
  }

  void visitString(const char *lhs) {
    Comparer<const char *> comparer(lhs);
    accept(comparer);
  }

  void visitRawJson(const char *lhsData, size_t lhsSize) {
    RawComparer comparer(lhsData, lhsSize);
    accept(comparer);
  }

  void visitNegativeInteger(UInt lhs) {
    NegativeIntegerComparer comparer(lhs);
    accept(comparer);
  }

  void visitPositiveInteger(UInt lhs) {
    Comparer<UInt> comparer(lhs);
    accept(comparer);
  }

  void visitBoolean(bool lhs) {
    Comparer<bool> comparer(lhs);
    accept(comparer);
  }

  void visitNull() {
    NullComparer comparer;
    accept(comparer);
  }

 private:
  template <typename TComparer>
  void accept(TComparer &comparer) {
    rhs.accept(comparer);
    switch (comparer.result) {
      case COMPARE_RESULT_GREATER:
        result = COMPARE_RESULT_LESS;
        break;
      case COMPARE_RESULT_LESS:
        result = COMPARE_RESULT_GREATER;
        break;
      default:
        result = comparer.result;
        break;
    }
  }
};

template <typename T1, typename T2>
CompareResult compare(const T1 &lhs, const T2 &rhs) {
  Comparer<T2> comparer(rhs);
  lhs.accept(comparer);
  return comparer.result;
}

inline int variantCompare(const VariantData *a, const VariantData *b) {
  return compare(VariantConstRef(a), VariantConstRef(b));
}

}  // namespace ARDUINOJSON_NAMESPACE
