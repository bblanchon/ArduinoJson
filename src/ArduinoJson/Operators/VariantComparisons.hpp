// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T, typename Enable = void>
struct Comparer;

template <typename T>
struct Comparer<T, typename enable_if<IsString<T>::value>::type> {
  T rhs;
  int result;

  explicit Comparer(T value) : rhs(value), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *lhs) {
    result = -adaptString(rhs).compare(lhs);
  }
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {
    result = adaptString(rhs).compare(NULL);
  }
};

template <typename T>
typename enable_if<is_signed<T>::value, int>::type sign(const T &value) {
  return value < 0 ? -1 : value > 0 ? 1 : 0;
}

template <typename T>
typename enable_if<is_unsigned<T>::value, int>::type sign(const T &value) {
  return value > 0 ? 1 : 0;
}

template <typename T>
struct Comparer<T, typename enable_if<is_integral<T>::value ||
                                      is_floating_point<T>::value>::type> {
  T rhs;
  int result;

  explicit Comparer(T value) : rhs(value), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float lhs) {
    result = sign(lhs - static_cast<Float>(rhs));
  }
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt lhs) {
    result = -sign(static_cast<T>(lhs) + rhs);
  }
  void visitPositiveInteger(UInt lhs) {
    result = static_cast<T>(lhs) < rhs ? -1 : static_cast<T>(lhs) > rhs ? 1 : 0;
  }
  void visitBoolean(bool) {}
  void visitNull() {}
};

template <>
struct Comparer<bool, void> {
  bool rhs;
  int result;

  explicit Comparer(bool value) : rhs(value), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool lhs) {
    result = static_cast<int>(lhs - rhs);
  }
  void visitNull() {}
};

#if ARDUINOJSON_HAS_NULLPTR
template <>
struct Comparer<decltype(nullptr), void> {
  int result;

  explicit Comparer(decltype(nullptr)) : result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {
    result = 0;
  }
};
#endif

template <typename TVariant>
class VariantComparisons {
 private:
  template <typename T>
  static int compare(TVariant lhs, const T &rhs) {
    Comparer<T> comparer(rhs);
    lhs.accept(comparer);
    return comparer.result;
  }

 public:
  // value == TVariant
  template <typename T>
  friend bool operator==(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) == 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator==(
      const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) == 0;
  }

  // TVariant == value
  template <typename T>
  friend bool operator==(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) == 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator==(
      TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) == 0;
  }

  // value != TVariant
  template <typename T>
  friend bool operator!=(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) != 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator!=(
      const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) != 0;
  }

  // TVariant != value
  template <typename T>
  friend bool operator!=(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) != 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator!=(
      TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) != 0;
  }

  // value < TVariant
  template <typename T>
  friend bool operator<(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) > 0;
  }
  template <typename T>
  friend bool operator<(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) > 0;
  }

  // TVariant < value
  template <typename T>
  friend bool operator<(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) < 0;
  }
  template <typename T>
  friend bool operator<(TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) < 0;
  }

  // value <= TVariant
  template <typename T>
  friend bool operator<=(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) >= 0;
  }
  template <typename T>
  friend bool operator<=(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) >= 0;
  }

  // TVariant <= value
  template <typename T>
  friend bool operator<=(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) <= 0;
  }
  template <typename T>
  friend bool operator<=(TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) <= 0;
  }

  // value > TVariant
  template <typename T>
  friend bool operator>(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) < 0;
  }
  template <typename T>
  friend bool operator>(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) < 0;
  }

  // TVariant > value
  template <typename T>
  friend bool operator>(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) > 0;
  }
  template <typename T>
  friend bool operator>(TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) > 0;
  }

  // value >= TVariant
  template <typename T>
  friend bool operator>=(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) <= 0;
  }
  template <typename T>
  friend bool operator>=(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) <= 0;
  }

  // TVariant >= value
  template <typename T>
  friend bool operator>=(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) >= 0;
  }
  template <typename T>
  friend bool operator>=(TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) >= 0;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
