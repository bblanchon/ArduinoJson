// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayRef.hpp>
#include <ArduinoJson/Document/JsonDocument.hpp>

namespace ARDUINOJSON_NAMESPACE {

// Copy a 1D array to a JsonArray
template <typename T, size_t N, typename TDestination>
inline typename enable_if<!is_array<T>::value &&
                              !is_base_of<JsonDocument, TDestination>::value,
                          bool>::type
copyArray(T (&src)[N], const TDestination& dst) {
  return copyArray(src, N, dst);
}

// Copy a 1D array to a JsonDocument
template <typename T, size_t N>
inline bool copyArray(T (&src)[N], JsonDocument& dst) {
  return copyArray(src, dst.to<ArrayRef>());
}

// Copy a 1D array to a JsonArray
template <typename T, typename TDestination>
inline typename enable_if<!is_array<T>::value &&
                              !is_base_of<JsonDocument, TDestination>::value,
                          bool>::type
copyArray(T* src, size_t len, const TDestination& dst) {
  bool ok = true;
  for (size_t i = 0; i < len; i++) {
    ok &= dst.add(src[i]);
  }
  return ok;
}

// Copy a 1D array to a JsonDocument
template <typename T>
inline bool copyArray(T* src, size_t len, JsonDocument& dst) {
  return copyArray(src, len, dst.to<ArrayRef>());
}

// Copy a 2D array to a JsonArray
template <typename T, size_t N1, size_t N2, typename TDestination>
inline typename enable_if<!is_base_of<JsonDocument, TDestination>::value,
                          bool>::type
copyArray(T (&src)[N1][N2], const TDestination& dst) {
  bool ok = true;
  for (size_t i = 0; i < N1; i++) {
    ArrayRef nestedArray = dst.createNestedArray();
    for (size_t j = 0; j < N2; j++) {
      ok &= nestedArray.add(src[i][j]);
    }
  }
  return ok;
}

// Copy a 2D array to a JsonDocument
template <typename T, size_t N1, size_t N2>
inline bool copyArray(T (&src)[N1][N2], JsonDocument& dst) {
  return copyArray(src, dst.to<ArrayRef>());
}

template <typename T>
class ArrayCopier1D : public Visitor<size_t> {
 public:
  ArrayCopier1D(T* destination, size_t capacity)
      : _destination(destination), _capacity(capacity) {}

  size_t visitArray(const CollectionData& array) {
    size_t size = 0;
    VariantSlot* slot = array.head();

    while (slot != 0 && size < _capacity) {
      _destination[size++] = variantAs<T>(slot->data());
      slot = slot->next();
    }
    return size;
  }

  size_t visitObject(const CollectionData&) {
    return 0;
  }

  size_t visitFloat(Float) {
    return 0;
  }

  size_t visitString(const char*) {
    return 0;
  }

  size_t visitRawJson(const char*, size_t) {
    return 0;
  }

  size_t visitNegativeInteger(UInt) {
    return 0;
  }

  size_t visitPositiveInteger(UInt) {
    return 0;
  }

  size_t visitBoolean(bool) {
    return 0;
  }

  size_t visitNull() {
    return 0;
  }

 private:
  T* _destination;
  size_t _capacity;
};

template <typename T, size_t N1, size_t N2>
class ArrayCopier2D : public Visitor<void> {
 public:
  ArrayCopier2D(T (*destination)[N1][N2]) : _destination(destination) {}

  void visitArray(const CollectionData& array) {
    VariantSlot* slot = array.head();
    size_t n = 0;
    while (slot != 0 && n < N1) {
      ArrayCopier1D<T> copier((*_destination)[n++], N2);
      variantAccept(slot->data(), copier);
      slot = slot->next();
    }
  }
  void visitObject(const CollectionData&) {}
  void visitFloat(Float) {}
  void visitString(const char*) {}
  void visitRawJson(const char*, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {}

 private:
  T (*_destination)[N1][N2];
  size_t _capacity1, _capacity2;
};

// Copy a JsonArray to a 1D array
template <typename TSource, typename T, size_t N>
inline typename enable_if<!is_array<T>::value, size_t>::type copyArray(
    const TSource& src, T (&dst)[N]) {
  return copyArray(src, dst, N);
}

// Copy a JsonArray to a 1D array
template <typename TSource, typename T>
inline size_t copyArray(const TSource& src, T* dst, size_t len) {
  ArrayCopier1D<T> copier(dst, len);

  return src.accept(copier);
}

// Copy a JsonArray to a 2D array
template <typename TSource, typename T, size_t N1, size_t N2>
inline void copyArray(const TSource& src, T (&dst)[N1][N2]) {
  ArrayCopier2D<T, N1, N2> copier(&dst);
  src.accept(copier);
}

}  // namespace ARDUINOJSON_NAMESPACE
