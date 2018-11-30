// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Configuration.hpp"
#include "../Numbers/parseFloat.hpp"
#include "../Numbers/parseInteger.hpp"
#include "VariantRef.hpp"

#include <string.h>  // for strcmp

namespace ARDUINOJSON_NAMESPACE {

inline bool VariantRef::set(ArrayRef array) const {
  return to<ArrayRef>().copyFrom(array);
}

inline bool VariantRef::set(const ArraySubscript& value) const {
  return set(value.as<VariantRef>());
}

inline bool VariantRef::set(ObjectRef object) const {
  return to<ObjectRef>().copyFrom(object);
}

template <typename TString>
inline bool VariantRef::set(const ObjectSubscript<TString>& value) const {
  return set(value.template as<VariantRef>());
}

inline bool VariantRef::set(VariantConstRef value) const {
  return variantCopy(_data, value._data, _memoryPool);
}

inline bool VariantRef::set(VariantRef value) const {
  return variantCopy(_data, value._data, _memoryPool);
}

template <typename T>
inline typename enable_if<is_same<T, ArrayRef>::value, T>::type VariantRef::as()
    const {
  return ArrayRef(_memoryPool, variantAsArray(_data));
}

template <typename T>
inline typename enable_if<is_same<T, ObjectRef>::value, T>::type
VariantRef::as() const {
  return ObjectRef(_memoryPool, variantAsObject(_data));
}

template <typename T>
inline typename enable_if<is_same<T, ArrayRef>::value, ArrayRef>::type
VariantRef::to() const {
  return ArrayRef(_memoryPool, variantToArray(_data));
}

template <typename T>
typename enable_if<is_same<T, ObjectRef>::value, ObjectRef>::type
VariantRef::to() const {
  return ObjectRef(_memoryPool, variantToObject(_data));
}

template <typename T>
typename enable_if<is_same<T, VariantRef>::value, VariantRef>::type
VariantRef::to() const {
  variantSetNull(_data);
  return *this;
}

template <typename Visitor>
inline void VariantRef::accept(Visitor& visitor) const {
  return VariantConstRef(_data).accept(visitor);
}

template <typename Visitor>
inline void VariantConstRef::accept(Visitor& visitor) const {
  if (!_data) return visitor.visitNull();

  switch (_data->type) {
    case JSON_FLOAT:
      return visitor.visitFloat(_data->content.asFloat);

    case JSON_ARRAY:
      return visitor.visitArray(ArrayConstRef(&_data->content.asArray));

    case JSON_OBJECT:
      return visitor.visitObject(ObjectConstRef(&_data->content.asObject));

    case JSON_LINKED_STRING:
    case JSON_OWNED_STRING:
      return visitor.visitString(_data->content.asString);

    case JSON_OWNED_RAW:
    case JSON_LINKED_RAW:
      return visitor.visitRawJson(_data->content.asRaw.data,
                                  _data->content.asRaw.size);

    case JSON_NEGATIVE_INTEGER:
      return visitor.visitNegativeInteger(_data->content.asInteger);

    case JSON_POSITIVE_INTEGER:
      return visitor.visitPositiveInteger(_data->content.asInteger);

    case JSON_BOOLEAN:
      return visitor.visitBoolean(_data->content.asInteger != 0);

    default:
      return visitor.visitNull();
  }
}

inline VariantConstRef VariantConstRef::operator[](size_t index) const {
  return ArrayConstRef(variantAsArray(_data))[index];
}

}  // namespace ARDUINOJSON_NAMESPACE
