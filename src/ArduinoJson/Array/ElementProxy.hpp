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

template <typename TArray>
class ElementProxy : public VariantOperators<ElementProxy<TArray> >,
                     public Visitable {
  typedef ElementProxy<TArray> this_type;

 public:
  FORCE_INLINE ElementProxy(TArray array, size_t index)
      : _array(array), _index(index) {}

  FORCE_INLINE this_type& operator=(const this_type& src) {
    getElement().set(src.as<VariantConstRef>());
    return *this;
  }

  // Replaces the value
  //
  // operator=(const TValue&)
  // TValue = bool, long, int, short, float, double, serialized, VariantRef,
  //          std::string, String, ArrayRef, ObjectRef
  template <typename T>
  FORCE_INLINE this_type& operator=(const T& src) {
    getElement().set(src);
    return *this;
  }
  //
  // operator=(TValue)
  // TValue = char*, const char*, const __FlashStringHelper*
  template <typename T>
  FORCE_INLINE this_type& operator=(T* src) {
    getElement().set(src);
    return *this;
  }

  FORCE_INLINE bool isNull() const {
    return getElement().isNull();
  }

  template <typename T>
  FORCE_INLINE typename VariantAs<T>::type as() const {
    return getElement().template as<T>();
  }

  template <typename T>
  FORCE_INLINE bool is() const {
    return getElement().template is<T>();
  }

  template <typename T>
  FORCE_INLINE typename VariantTo<T>::type to() const {
    return getElement().template to<T>();
  }

  // Replaces the value
  //
  // bool set(const TValue&)
  // TValue = bool, long, int, short, float, double, serialized, VariantRef,
  //          std::string, String, ArrayRef, ObjectRef
  template <typename TValue>
  FORCE_INLINE bool set(const TValue& value) const {
    return getElement().set(value);
  }
  //
  // bool set(TValue)
  // TValue = char*, const char*, const __FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE bool set(TValue* value) const {
    return getElement().set(value);
  }

  template <typename Visitor>
  void accept(Visitor& visitor) const {
    return getElement().accept(visitor);
  }

  FORCE_INLINE size_t size() const {
    return getElement().size();
  }

  template <typename TNestedKey>
  VariantRef get(TNestedKey* key) const {
    return getElement().get(key);
  }

  template <typename TNestedKey>
  VariantRef get(const TNestedKey& key) const {
    return getElement().get(key);
  }

  template <typename TNestedKey>
  VariantRef getOrCreate(TNestedKey* key) const {
    return getElement().getOrCreate(key);
  }

  template <typename TNestedKey>
  VariantRef getOrCreate(const TNestedKey& key) const {
    return getElement().getOrCreate(key);
  }

  using ArrayShortcuts<ElementProxy>::add;
  VariantRef add() const {
    return getElement().add();
  }

 private:
  FORCE_INLINE VariantRef getElement() const {
    return _array.get(_index);
  }

  TArray _array;
  const size_t _index;
};

template <typename TArray>
inline ElementProxy<const TArray&> ArrayShortcuts<TArray>::operator[](
    size_t index) const {
  return ElementProxy<const TArray&>(*impl(), index);
}

}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
