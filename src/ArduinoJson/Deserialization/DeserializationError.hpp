// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_STD_STREAM
#include <ostream>
#endif

namespace ArduinoJson {

class DeserializationError {
 public:
  enum Code {
    Ok,
    IncompleteInput,
    InvalidInput,
    NoMemory,
    NotSupported,
    TooDeep
  };

  DeserializationError() {}
  DeserializationError(Code code) : _code(code) {}

  friend bool operator==(const DeserializationError& err, Code code) {
    return err._code == code;
  }

  friend bool operator==(Code code, const DeserializationError& err) {
    return err._code == code;
  }

  friend bool operator!=(const DeserializationError& err, Code code) {
    return err._code != code;
  }

  friend bool operator!=(Code code, const DeserializationError& err) {
    return err._code != code;
  }

  operator bool() const {
    return _code != Ok;
  }

  const char* c_str() const {
    switch (_code) {
      case Ok:
        return "Ok";
      case TooDeep:
        return "TooDeep";
      case NoMemory:
        return "NoMemory";
      case InvalidInput:
        return "InvalidInput";
      case IncompleteInput:
        return "IncompleteInput";
      case NotSupported:
        return "NotSupported";
      default:
        return "???";
    }
  }

 private:
  Code _code;
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

}  // namespace ArduinoJson
