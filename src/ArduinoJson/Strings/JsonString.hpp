// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Misc/SafeBoolIdiom.hpp>

#if ARDUINOJSON_ENABLE_STD_STREAM
#  include <ostream>
#endif

namespace ARDUINOJSON_NAMESPACE {

// A string.
// https://arduinojson.org/v6/api/jsonstring/
class JsonString : public SafeBoolIdom<JsonString> {
 public:
  enum Ownership { Copied, Linked };

  JsonString() : _data(0), _size(0), _ownership(Linked) {}

  JsonString(const char* data, Ownership ownership = Linked)
      : _data(data), _size(data ? ::strlen(data) : 0), _ownership(ownership) {}

  JsonString(const char* data, size_t sz, Ownership ownership = Linked)
      : _data(data), _size(sz), _ownership(ownership) {}

  // Returns a pointer to the characters.
  const char* c_str() const {
    return _data;
  }

  // Returns true if the string is null.
  bool isNull() const {
    return !_data;
  }

  // Returns true if the string is stored by address.
  // Returns false if the string is stored by copy.
  bool isLinked() const {
    return _ownership == Linked;
  }

  // Returns length of the string.
  size_t size() const {
    return _size;
  }

  // safe bool idiom
  operator bool_type() const {
    return _data ? safe_true() : safe_false();
  }

  // Returns true if strings are equal.
  friend bool operator==(JsonString lhs, JsonString rhs) {
    if (lhs._size != rhs._size)
      return false;
    if (lhs._data == rhs._data)
      return true;
    if (!lhs._data)
      return false;
    if (!rhs._data)
      return false;
    return memcmp(lhs._data, rhs._data, lhs._size) == 0;
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
  const char* _data;
  size_t _size;
  Ownership _ownership;
};

}  // namespace ARDUINOJSON_NAMESPACE
