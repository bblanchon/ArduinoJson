// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Configuration.hpp"
#include "../Operators/VariantOperators.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ARDUINOJSON_NAMESPACE {
class ArraySubscript : public VariantOperators<ArraySubscript>,
                       public Visitable {
 public:
  FORCE_INLINE ArraySubscript(ArrayRef array, size_t index)
      : _array(array), _index(index) {}

  FORCE_INLINE ArraySubscript& operator=(const ArraySubscript& src) {
    get_impl().set(src.as<VariantConstRef>());
    return *this;
  }

  // Replaces the value
  //
  // operator=(const TValue&)
  // TValue = bool, long, int, short, float, double, serialized, VariantRef,
  //          std::string, String, ArrayRef, ObjectRef
  template <typename T>
  FORCE_INLINE ArraySubscript& operator=(const T& src) {
    get_impl().set(src);
    return *this;
  }
  //
  // operator=(TValue)
  // TValue = char*, const char*, const __FlashStringHelper*
  template <typename T>
  FORCE_INLINE ArraySubscript& operator=(T* src) {
    get_impl().set(src);
    return *this;
  }

  FORCE_INLINE bool isNull() const {
    return get_impl().isNull();
  }

  template <typename T>
  FORCE_INLINE typename VariantAs<T>::type as() const {
    return get_impl().as<T>();
  }

  template <typename T>
  FORCE_INLINE bool is() const {
    return get_impl().is<T>();
  }

  template <typename T>
  FORCE_INLINE typename VariantTo<T>::type to() const {
    return get_impl().to<T>();
  }

  // Replaces the value
  //
  // bool set(const TValue&)
  // TValue = bool, long, int, short, float, double, serialized, VariantRef,
  //          std::string, String, ArrayRef, ObjectRef
  template <typename TValue>
  FORCE_INLINE bool set(const TValue& value) const {
    return get_impl().set(value);
  }
  //
  // bool set(TValue)
  // TValue = char*, const char*, const __FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE bool set(TValue* value) const {
    return get_impl().set(value);
  }

  template <typename Visitor>
  void accept(Visitor& visitor) const {
    return get_impl().accept(visitor);
  }

  FORCE_INLINE size_t size() const {
    return get_impl().size();
  }

 private:
  FORCE_INLINE VariantRef get_impl() const {
    return _array.get(_index);
  }

  ArrayRef _array;
  const size_t _index;
};

template <typename TImpl>
inline ArraySubscript VariantSubscripts<TImpl>::operator[](size_t index) const {
  return impl()->template as<ArrayRef>()[index];
}

inline ArraySubscript ArrayRef::operator[](size_t index) const {
  return ArraySubscript(*this, index);
}
}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
