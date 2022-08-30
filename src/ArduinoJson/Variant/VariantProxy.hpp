// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>
#include <ArduinoJson/Variant/VariantShortcuts.hpp>
#include <ArduinoJson/Variant/VariantTo.hpp>

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4522)
#endif

namespace ARDUINOJSON_NAMESPACE {

template <typename TDataSource>
class VariantProxy : public VariantShortcuts<VariantProxy<TDataSource> >,
                     public VariantOperators<VariantProxy<TDataSource> >,
                     public VariantTag {
  friend class VariantAttorney;

 public:
  explicit FORCE_INLINE VariantProxy(TDataSource source) : _source(source) {}

  // Copy-constructor required because of user-defined copy-assignment operator
  FORCE_INLINE VariantProxy(const VariantProxy& src) : _source(src._source) {}

  FORCE_INLINE VariantProxy& operator=(const VariantProxy& src) {
    getOrAddUpstreamVariant().set(src);
    return *this;
  }

  template <typename T>
  FORCE_INLINE VariantProxy& operator=(const T& src) {
    getOrAddUpstreamVariant().set(src);
    return *this;
  }

  template <typename T>
  FORCE_INLINE VariantProxy& operator=(T* src) {
    getOrAddUpstreamVariant().set(src);
    return *this;
  }

  FORCE_INLINE void clear() const {
    getUpstreamVariant().clear();
  }

  FORCE_INLINE bool isNull() const {
    return getUpstreamVariantConst().isNull();
  }

  template <typename T>
  FORCE_INLINE typename enable_if<!is_same<T, char*>::value &&
                                      !ConverterNeedsWriteableRef<T>::value,
                                  T>::type
  as() const {
    return getUpstreamVariantConst().template as<T>();
  }

  template <typename T>
  FORCE_INLINE typename enable_if<ConverterNeedsWriteableRef<T>::value, T>::type
  as() const {
    return getUpstreamVariant().template as<T>();
  }

  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, char*>::value, const char*>::type
  ARDUINOJSON_DEPRECATED("Replace as<char*>() with as<const char*>()")
      as() const {
    return as<const char*>();
  }

  template <typename T>
  FORCE_INLINE operator T() const {
    return as<T>();
  }

  template <typename T>
  FORCE_INLINE typename VariantTo<T>::type to() {
    return getOrAddUpstreamVariant().template to<T>();
  }

  template <typename T>
  FORCE_INLINE
      typename enable_if<ConverterNeedsWriteableRef<T>::value, bool>::type
      is() const {
    return getUpstreamVariant().template is<T>();
  }

  template <typename T>
  FORCE_INLINE
      typename enable_if<!ConverterNeedsWriteableRef<T>::value, bool>::type
      is() const {
    return getUpstreamVariantConst().template is<T>();
  }

  FORCE_INLINE void shallowCopy(VariantConstRef value) {
    getOrAddUpstreamVariant().shallowCopy(value);
  }

  template <typename T>
  FORCE_INLINE bool set(const T& value) {
    return getOrAddUpstreamVariant().set(value);
  }

  template <typename T>
  FORCE_INLINE bool set(T* value) {
    return getOrAddUpstreamVariant().set(value);
  }

  FORCE_INLINE size_t size() const {
    return getUpstreamVariantConst().size();
  }

  FORCE_INLINE size_t memoryUsage() const {
    return getUpstreamVariantConst().memoryUsage();
  }

  FORCE_INLINE VariantRef add() const {
    return getOrAddUpstreamVariant().add();
  }

  using ArrayShortcuts<VariantProxy<TDataSource> >::add;

  FORCE_INLINE void remove(size_t index) const {
    getUpstreamVariant().remove(index);
  }
  // remove(char*) const
  // remove(const char*) const
  // remove(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value>::type remove(
      TChar* key) const {
    getUpstreamVariant().remove(key);
  }
  // remove(const std::string&) const
  // remove(const String&) const
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value>::type remove(
      const TString& key) const {
    getUpstreamVariant().remove(key);
  }

 protected:
  FORCE_INLINE MemoryPool* getPool() const {
    return _source.getPool();
  }

  FORCE_INLINE VariantData* getData() const {
    return _source.getData();
  }

  FORCE_INLINE VariantData* getOrCreateData() const {
    return _source.getOrCreateData();
  }

 private:
  FORCE_INLINE VariantRef getUpstreamVariant() const {
    return VariantRef(getPool(), getData());
  }

  FORCE_INLINE VariantConstRef getUpstreamVariantConst() const {
    return VariantConstRef(getData());
  }

  FORCE_INLINE VariantRef getOrAddUpstreamVariant() const {
    return VariantRef(getPool(), getOrCreateData());
  }

  friend void convertToJson(const VariantProxy& src, VariantRef dst) {
    dst.set(src.getUpstreamVariantConst());
  }

  const TDataSource _source;
};

}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#  pragma warning(pop)
#endif
