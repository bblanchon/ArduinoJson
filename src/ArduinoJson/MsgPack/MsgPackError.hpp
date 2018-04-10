// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

class MsgPackError {
 public:
  enum Code { Ok, NotSupported, NoMemory, NotAnArray, NotAnObject, TooDeep };

  MsgPackError() {}

  MsgPackError(Code code) : _code(code) {}

  operator bool() const {
    return _code != Ok;
  }

  friend bool operator==(const MsgPackError& err, Code code) {
    return err._code == code;
  }

  friend bool operator==(Code code, const MsgPackError& err) {
    return err._code == code;
  }

  friend bool operator!=(const MsgPackError& err, Code code) {
    return err._code != code;
  }

  friend bool operator!=(Code code, const MsgPackError& err) {
    return err._code != code;
  }

  const char* c_str() const {
    switch (_code) {
      case Ok:
        return "Ok";
      case NotSupported:
        return "NotSupported";
      case NoMemory:
        return "NoMemory";
      case NotAnArray:
        return "NotAnArray";
      case NotAnObject:
        return "NotAnObject";
      case TooDeep:
        return "TooDeep";
      default:
        return "???";
    }
  }

 private:
  Code _code;
};

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& os, const MsgPackError& err) {
  os << err.c_str();
  return os;
}
#endif

}  // namespace ArduinoJson
