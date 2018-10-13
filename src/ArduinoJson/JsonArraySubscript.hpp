// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Configuration.hpp"
#include "JsonVariantBase.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ARDUINOJSON_NAMESPACE {
class JsonArraySubscript : public JsonVariantBase<JsonArraySubscript>,
                           public Visitable {
 public:
  FORCE_INLINE JsonArraySubscript(JsonArray array, size_t index)
      : _array(array), _index(index) {}

  FORCE_INLINE JsonArraySubscript& operator=(const JsonArraySubscript& src) {
    get_impl().set(src.as<JsonVariantConst>());
    return *this;
  }

  // Replaces the value
  //
  // operator=(const TValue&)
  // TValue = bool, long, int, short, float, double, serialized, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename T>
  FORCE_INLINE JsonArraySubscript& operator=(const T& src) {
    get_impl().set(src);
    return *this;
  }
  //
  // operator=(TValue)
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename T>
  FORCE_INLINE JsonArraySubscript& operator=(T* src) {
    get_impl().set(src);
    return *this;
  }

  FORCE_INLINE bool isNull() const {
    return _index >= _array.size();
  }

  template <typename T>
  FORCE_INLINE typename JsonVariantAs<T>::type as() const {
    return _array.get<T>(_index);
  }

  template <typename T>
  FORCE_INLINE bool is() const {
    return _array.is<T>(_index);
  }

  template <typename T>
  FORCE_INLINE typename JsonVariantTo<T>::type to() const {
    return _array.get<JsonVariant>(_index).to<T>();
  }

  // Replaces the value
  //
  // bool set(const TValue&)
  // TValue = bool, long, int, short, float, double, serialized, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue>
  FORCE_INLINE bool set(const TValue& value) const {
    return get_impl().set(value);
  }
  //
  // bool set(TValue)
  // TValue = char*, const char*, const FlashStringHelper*
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
  JsonVariant get_impl() const {
    return _array.get<JsonVariant>(_index);
  }

  JsonArray _array;
  const size_t _index;
};

template <typename TImpl>
inline JsonArraySubscript JsonVariantSubscripts<TImpl>::operator[](
    size_t index) const {
  return impl()->template as<JsonArray>()[index];
}

inline JsonArraySubscript JsonArray::operator[](size_t index) const {
  return JsonArraySubscript(*this, index);
}
}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
