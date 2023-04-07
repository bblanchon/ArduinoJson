// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_STD_STREAM
#  include <ostream>
#endif

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A string.
// https://arduinojson.org/v6/api/jsonstring/
class JsonString {
 public:
  enum Ownership { Copied, Linked };

  JsonString() : data_(0), size_(0), ownership_(Linked) {}

  JsonString(const char* data, Ownership ownership = Linked)
      : data_(data), size_(data ? ::strlen(data) : 0), ownership_(ownership) {}

  JsonString(const char* data, size_t sz, Ownership ownership = Linked)
      : data_(data), size_(sz), ownership_(ownership) {}

  // Returns a pointer to the characters.
  const char* c_str() const {
    return data_;
  }

  // Returns true if the string is null.
  bool isNull() const {
    return !data_;
  }

  // Returns true if the string is stored by address.
  // Returns false if the string is stored by copy.
  bool isLinked() const {
    return ownership_ == Linked;
  }

  // Returns length of the string.
  size_t size() const {
    return size_;
  }

  // Returns true if the string is non-null
  explicit operator bool() const {
    return data_ != 0;
  }

  // Returns true if strings are equal.
  friend bool operator==(JsonString lhs, JsonString rhs) {
    if (lhs.size_ != rhs.size_)
      return false;
    if (lhs.data_ == rhs.data_)
      return true;
    if (!lhs.data_)
      return false;
    if (!rhs.data_)
      return false;
    return memcmp(lhs.data_, rhs.data_, lhs.size_) == 0;
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
  const char* data_;
  size_t size_;
  Ownership ownership_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
