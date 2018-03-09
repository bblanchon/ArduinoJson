// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

class JsonError {
 public:
  enum Code {
    Ok,
    OpeningBraceExpected,
    ClosingBraceExpected,
    OpeningBracketExpected,
    ClosingBracketExpected,
    ColonExpected,
    TooDeep,
    NoMemory
  };

  JsonError(Code code) : _code(code) {}

  bool operator==(Code code) const {
    return _code == code;
  }

  bool operator!=(Code code) const {
    return _code != code;
  }

  operator bool() const {
    return _code != Ok;
  }

  const char* c_str() const {
    return to_string(_code);
  }

  friend const char* to_string(const JsonError err) {
    return to_string(err._code);
  }

  friend const char* to_string(JsonError::Code code) {
    switch (code) {
      case Ok:
        return "Ok";
      case OpeningBraceExpected:
        return "OpeningBraceExpected";
      case ClosingBraceExpected:
        return "ClosingBraceExpected";
      case OpeningBracketExpected:
        return "OpeningBracketExpected";
      case ClosingBracketExpected:
        return "ClosingBracketExpected";
      case ColonExpected:
        return "ColonExpected";
      case TooDeep:
        return "TooDeep";
      case NoMemory:
        return "NoMemory";
      default:
        return "???";
    }
  }

 private:
  Code _code;
};

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& s, const JsonError& e) {
  s << to_string(e);
  return s;
}

inline std::ostream& operator<<(std::ostream& s, JsonError::Code e) {
  s << to_string(e);
  return s;
}
#endif

}  // namespace ArduinoJson
