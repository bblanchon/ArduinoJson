// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

class JsonError {
 public:
  enum Code { Ok, TooDeep, NoMemory, InvalidInput };

  JsonError(Code code) : _code(code) {}

  friend bool operator==(const JsonError& err, Code code) {
    return err._code == code;
  }

  friend bool operator==(Code code, const JsonError& err) {
    return err._code == code;
  }

  friend bool operator!=(const JsonError& err, Code code) {
    return err._code != code;
  }

  friend bool operator!=(Code code, const JsonError& err) {
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
      default:
        return "???";
    }
  }

 private:
  Code _code;
};

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& s, const JsonError& e) {
  s << e.c_str();
  return s;
}

inline std::ostream& operator<<(std::ostream& s, JsonError::Code c) {
  s << JsonError(c).c_str();
  return s;
}
#endif

}  // namespace ArduinoJson
