// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/Converter.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>
#include <ArduinoJson/Variant/VariantShortcuts.hpp>
#include <ArduinoJson/Variant/VariantTo.hpp>

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4522)
#endif

namespace ARDUINOJSON_NAMESPACE {

template <typename TObject, typename TStringRef>
class MemberProxy : public VariantOperators<MemberProxy<TObject, TStringRef> >,
                    public VariantShortcuts<MemberProxy<TObject, TStringRef> >,
                    public Visitable,
                    public VariantTag {
  typedef MemberProxy<TObject, TStringRef> this_type;

 public:
  typedef VariantRef variant_type;

  FORCE_INLINE MemberProxy(TObject variant, TStringRef key)
      : _object(variant), _key(key) {}

  FORCE_INLINE MemberProxy(const MemberProxy &src)
      : _object(src._object), _key(src._key) {}

  FORCE_INLINE operator VariantConstRef() const {
    return getUpstreamMemberConst();
  }

  FORCE_INLINE this_type &operator=(const this_type &src) {
    getOrAddUpstreamMember().set(src);
    return *this;
  }

  template <typename TValue>
  FORCE_INLINE typename enable_if<!is_array<TValue>::value, this_type &>::type
  operator=(const TValue &src) {
    getOrAddUpstreamMember().set(src);
    return *this;
  }

  // operator=(char*)
  // operator=(const char*)
  // operator=(const __FlashStringHelper*)
  template <typename TChar>
  FORCE_INLINE this_type &operator=(TChar *src) {
    getOrAddUpstreamMember().set(src);
    return *this;
  }

  FORCE_INLINE void clear() const {
    getUpstreamMember().clear();
  }

  FORCE_INLINE bool isNull() const {
    return getUpstreamMemberConst().isNull();
  }

  template <typename T>
  FORCE_INLINE typename enable_if<!is_same<T, char *>::value &&
                                      !ConverterNeedsWriteableRef<T>::value,
                                  T>::type
  as() const {
    return getUpstreamMemberConst().template as<T>();
  }

  template <typename T>
  FORCE_INLINE typename enable_if<ConverterNeedsWriteableRef<T>::value, T>::type
  as() const {
    return getUpstreamMember().template as<T>();
  }

  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, char *>::value, const char *>::type
  ARDUINOJSON_DEPRECATED("Replace as<char*>() with as<const char*>()")
      as() const {
    return as<const char *>();
  }

  template <typename T>
  FORCE_INLINE operator T() const {
    return as<T>();
  }

  template <typename T>
  FORCE_INLINE
      typename enable_if<ConverterNeedsWriteableRef<T>::value, bool>::type
      is() const {
    return getUpstreamMember().template is<T>();
  }

  template <typename T>
  FORCE_INLINE
      typename enable_if<!ConverterNeedsWriteableRef<T>::value, bool>::type
      is() const {
    return getUpstreamMemberConst().template is<T>();
  }

  FORCE_INLINE size_t size() const {
    return getUpstreamMemberConst().size();
  }

  FORCE_INLINE size_t memoryUsage() const {
    return getUpstreamMemberConst().memoryUsage();
  }

  FORCE_INLINE void remove(size_t index) const {
    getUpstreamMember().remove(index);
  }
  // remove(char*) const
  // remove(const char*) const
  // remove(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar *>::value>::type remove(
      TChar *key) const {
    getUpstreamMember().remove(key);
  }
  // remove(const std::string&) const
  // remove(const String&) const
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value>::type remove(
      const TString &key) const {
    getUpstreamMember().remove(key);
  }

  FORCE_INLINE void link(VariantConstRef value) {
    getOrAddUpstreamMember().link(value);
  }

  template <typename TValue>
  FORCE_INLINE typename VariantTo<TValue>::type to() {
    return getOrAddUpstreamMember().template to<TValue>();
  }

  template <typename TValue>
  FORCE_INLINE bool set(const TValue &value) {
    return getOrAddUpstreamMember().set(value);
  }

  // set(char*) const
  // set(const char*) const
  // set(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE bool set(TChar *value) {
    return getOrAddUpstreamMember().set(value);
  }

  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor &visitor) const {
    return getUpstreamMemberConst().accept(visitor);
  }

  FORCE_INLINE VariantRef addElement() const {
    return getOrAddUpstreamMember().addElement();
  }

  FORCE_INLINE VariantRef getElement(size_t index) const {
    return getUpstreamMember().getElement(index);
  }

  FORCE_INLINE VariantConstRef getElementConst(size_t index) const {
    return getUpstreamMemberConst().getElementConst(index);
  }

  FORCE_INLINE VariantRef getOrAddElement(size_t index) const {
    return getOrAddUpstreamMember().getOrAddElement(index);
  }

  // getMember(char*) const
  // getMember(const char*) const
  // getMember(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantRef getMember(TChar *key) const {
    return getUpstreamMember().getMember(key);
  }

  // getMember(const std::string&) const
  // getMember(const String&) const
  template <typename TString>
  FORCE_INLINE VariantRef getMember(const TString &key) const {
    return getUpstreamMember().getMember(key);
  }

  // getMemberConst(char*) const
  // getMemberConst(const char*) const
  // getMemberConst(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantConstRef getMemberConst(TChar *key) const {
    return getUpstreamMemberConst().getMemberConst(key);
  }

  // getMemberConst(const std::string&) const
  // getMemberConst(const String&) const
  template <typename TString>
  FORCE_INLINE VariantConstRef getMemberConst(const TString &key) const {
    return getUpstreamMemberConst().getMemberConst(key);
  }

  // getOrAddMember(char*) const
  // getOrAddMember(const char*) const
  // getOrAddMember(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantRef getOrAddMember(TChar *key) const {
    return getOrAddUpstreamMember().getOrAddMember(key);
  }

  // getOrAddMember(const std::string&) const
  // getOrAddMember(const String&) const
  template <typename TString>
  FORCE_INLINE VariantRef getOrAddMember(const TString &key) const {
    return getOrAddUpstreamMember().getOrAddMember(key);
  }

 private:
  FORCE_INLINE VariantRef getUpstreamMember() const {
    return _object.getMember(_key);
  }

  FORCE_INLINE VariantConstRef getUpstreamMemberConst() const {
    return _object.getMemberConst(_key);
  }

  FORCE_INLINE VariantRef getOrAddUpstreamMember() const {
    return _object.getOrAddMember(_key);
  }

  friend void convertToJson(const this_type &src, VariantRef dst) {
    dst.set(src.getUpstreamMemberConst());
  }

  TObject _object;
  TStringRef _key;
};

}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#  pragma warning(pop)
#endif
