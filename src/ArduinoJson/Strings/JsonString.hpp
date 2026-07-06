// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/Adapters/RamString.hpp>

#if ARDUINOJSON_ENABLE_STD_STREAM
#  include <ostream>
#endif

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A string.
// https://arduinojson.org/v7/api/jsonstring/
class JsonString {
  friend struct detail::StringAdapter<JsonString>;

 public:
  JsonString() : str_(nullptr, 0) {}

  JsonString(const char* data) : str_(data, data ? ::strlen(data) : 0) {}

  ARDUINOJSON_DEPRECATED(
      "ArduinoJson doesn't differentiate between static and dynamic strings "
      "anymore. Remove the second argument to fix this warning.")
  JsonString(const char* data, bool) : JsonString(data) {}

  template <typename TSize,
            detail::enable_if_t<detail::is_integral<TSize>::value &&
                                    !detail::is_same<TSize, bool>::value,
                                int> = 0>
  JsonString(const char* data, TSize sz) : str_(data, size_t(sz)) {}

  ARDUINOJSON_DEPRECATED(
      "ArduinoJson doesn't differentiate between static and dynamic strings "
      "anymore. Remove the third argument to fix this warning.")
  JsonString(const char* data, size_t sz, bool) : JsonString(data, sz) {}

  // Returns a pointer to the characters.
  const char* c_str() const {
    return str_.data();
  }

  // Returns true if the string is null.
  bool isNull() const {
    return str_.isNull();
  }

  // Deprecated: always returns false.
  ARDUINOJSON_DEPRECATED("The isStatic() was removed in v7.5")
  bool isStatic() const {
    return false;
  }

  // Returns length of the string.
  size_t size() const {
    return str_.size();
  }

  // Returns true if the string is non-null
  explicit operator bool() const {
    return str_.data() != 0;
  }

  // Returns true if strings are equal.
  friend bool operator==(JsonString lhs, JsonString rhs) {
    if (lhs.size() != rhs.size())
      return false;
    if (lhs.c_str() == rhs.c_str())
      return true;
    if (!lhs.c_str())
      return false;
    if (!rhs.c_str())
      return false;
    return memcmp(lhs.c_str(), rhs.c_str(), lhs.size()) == 0;
  }

  // Returns true if strings differs.
  friend bool operator!=(JsonString lhs, JsonString rhs) {
    return !(lhs == rhs);
  }

#if ARDUINOJSON_ENABLE_STD_STREAM
  friend std::ostream& operator<<(std::ostream& lhs, const JsonString& rhs) {
    lhs.write(rhs.c_str(), static_cast<std::streamsize>(rhs.size()));
    return lhs;
  }
#endif

 private:
  detail::RamString str_;
};

namespace detail {
template <>
struct StringAdapter<JsonString> {
  using AdaptedString = RamString;

  static const AdaptedString& adapt(const JsonString& s) {
    return s.str_;
  }
};
}  // namespace detail

ARDUINOJSON_END_PUBLIC_NAMESPACE
