// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/pgmspace_generic.hpp>
#include <ArduinoJson/Polyfills/preprocessor.hpp>

#if ARDUINOJSON_ENABLE_STD_STREAM
#  include <ostream>
#endif

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class DeserializationError {
 public:
  enum Code {
    Ok,
    EmptyInput,
    IncompleteInput,
    InvalidInput,
    NoMemory,
    TooDeep
  };

  DeserializationError() {}
  DeserializationError(Code c) : code_(c) {}

  // Compare with DeserializationError
  friend bool operator==(const DeserializationError& lhs,
                         const DeserializationError& rhs) {
    return lhs.code_ == rhs.code_;
  }
  friend bool operator!=(const DeserializationError& lhs,
                         const DeserializationError& rhs) {
    return lhs.code_ != rhs.code_;
  }

  // Compare with Code
  friend bool operator==(const DeserializationError& lhs, Code rhs) {
    return lhs.code_ == rhs;
  }
  friend bool operator==(Code lhs, const DeserializationError& rhs) {
    return lhs == rhs.code_;
  }
  friend bool operator!=(const DeserializationError& lhs, Code rhs) {
    return lhs.code_ != rhs;
  }
  friend bool operator!=(Code lhs, const DeserializationError& rhs) {
    return lhs != rhs.code_;
  }

  // Returns true if there is an error
  explicit operator bool() const {
    return code_ != Ok;
  }

  // Returns internal enum, useful for switch statement
  Code code() const {
    return code_;
  }

  const char* c_str() const {
    static const char* messages[] = {
        "Ok",           "EmptyInput", "IncompleteInput",
        "InvalidInput", "NoMemory",   "TooDeep"};
    ARDUINOJSON_ASSERT(static_cast<size_t>(code_) <
                       sizeof(messages) / sizeof(messages[0]));
    return messages[code_];
  }

#if ARDUINOJSON_ENABLE_PROGMEM
  const __FlashStringHelper* f_str() const {
    ARDUINOJSON_DEFINE_PROGMEM_ARRAY(char, s0, "Ok");
    ARDUINOJSON_DEFINE_PROGMEM_ARRAY(char, s1, "EmptyInput");
    ARDUINOJSON_DEFINE_PROGMEM_ARRAY(char, s2, "IncompleteInput");
    ARDUINOJSON_DEFINE_PROGMEM_ARRAY(char, s3, "InvalidInput");
    ARDUINOJSON_DEFINE_PROGMEM_ARRAY(char, s4, "NoMemory");
    ARDUINOJSON_DEFINE_PROGMEM_ARRAY(char, s5, "TooDeep");
    ARDUINOJSON_DEFINE_PROGMEM_ARRAY(const char*, messages,
                                     {s0, s1, s2, s3, s4, s5});
    return reinterpret_cast<const __FlashStringHelper*>(
        detail::pgm_read(messages + code_));
  }
#endif

 private:
  Code code_;
};

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& s,
                                const DeserializationError& e) {
  s << e.c_str();
  return s;
}

inline std::ostream& operator<<(std::ostream& s, DeserializationError::Code c) {
  s << DeserializationError(c).c_str();
  return s;
}
#endif

ARDUINOJSON_END_PUBLIC_NAMESPACE
