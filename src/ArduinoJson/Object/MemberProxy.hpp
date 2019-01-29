// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Configuration.hpp"
#include "../Operators/VariantOperators.hpp"
#include "../Polyfills/type_traits.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ARDUINOJSON_NAMESPACE {

template <typename TObject, typename TStringRef>
class MemberProxy : public VariantOperators<MemberProxy<TObject, TStringRef> >,
                    public Visitable {
  typedef MemberProxy<TObject, TStringRef> this_type;

 public:
  FORCE_INLINE MemberProxy(TObject variant, TStringRef key)
      : _object(variant), _key(key) {}

  FORCE_INLINE operator VariantConstRef() const {
    return getMember();
  }

  FORCE_INLINE this_type &operator=(const this_type &src) {
    getOrCreateMember().set(src);
    return *this;
  }

  template <typename TValue>
  FORCE_INLINE typename enable_if<!is_array<TValue>::value, this_type &>::type
  operator=(const TValue &src) {
    getOrCreateMember().set(src);
    return *this;
  }

  // operator=(char*) const
  // operator=(const char*) const
  // operator=(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE this_type &operator=(TChar *src) {
    getOrCreateMember().set(src);
    return *this;
  }

  FORCE_INLINE bool isNull() const {
    return getMember().isNull();
  }

  template <typename TValue>
  FORCE_INLINE typename VariantAs<TValue>::type as() const {
    return getMember().template as<TValue>();
  }

  template <typename TValue>
  FORCE_INLINE bool is() const {
    return getMember().template is<TValue>();
  }

  template <typename TValue>
  FORCE_INLINE typename VariantTo<TValue>::type to() {
    return getOrCreateMember().template to<TValue>();
  }

  template <typename TValue>
  FORCE_INLINE typename enable_if<!is_array<TValue>::value, bool>::type set(
      const TValue &value) {
    return getOrCreateMember().set(value);
  }

  // set(char*) const
  // set(const char*) const
  // set(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE bool set(const TChar *value) {
    return getOrCreateMember().set(value);
  }

  template <typename Visitor>
  void accept(Visitor &visitor) const {
    return getMember().accept(visitor);
  }

  using ArrayShortcuts<MemberProxy>::add;
  FORCE_INLINE VariantRef add() const {
    return getOrCreateMember().add();
  }

  // get(char*) const
  // get(const char*) const
  // get(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantRef get(TChar *key) const {
    return getMember().get(key);
  }

  // get(const std::string&) const
  // get(const String&) const
  template <typename TString>
  FORCE_INLINE VariantRef get(const TString &key) const {
    return getMember().get(key);
  }

  // getOrCreate(char*) const
  // getOrCreate(const char*) const
  // getOrCreate(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantRef getOrCreate(TChar *key) const {
    return getOrCreateMember().getOrCreate(key);
  }

  // getOrCreate(const std::string&) const
  // getOrCreate(const String&) const
  template <typename TString>
  FORCE_INLINE VariantRef getOrCreate(const TString &key) const {
    return getOrCreateMember().getOrCreate(key);
  }

 private:
  FORCE_INLINE VariantRef getMember() const {
    return _object.get(_key);
  }

  FORCE_INLINE VariantRef getOrCreateMember() const {
    return _object.getOrCreate(_key);
  }

  TObject _object;
  TStringRef _key;
};

template <typename TObject>
template <typename TString>
inline typename enable_if<IsString<TString>::value,
                          MemberProxy<const TObject &, const TString &> >::type
    ObjectShortcuts<TObject>::operator[](const TString &key) const {
  return MemberProxy<const TObject &, const TString &>(*impl(), key);
}

template <typename TObject>
template <typename TString>
inline typename enable_if<IsString<TString *>::value,
                          MemberProxy<const TObject &, TString *> >::type
    ObjectShortcuts<TObject>::operator[](TString *key) const {
  return MemberProxy<const TObject &, TString *>(*impl(), key);
}

}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
