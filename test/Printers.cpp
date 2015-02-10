// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "Printers.hpp"
#include <ArduinoJson/JsonArray.hpp>

class StreamPrintAdapter : public Print {
 public:
  explicit StreamPrintAdapter(std::ostream& os) : _os(os) {}

  virtual size_t write(uint8_t c) {
    _os << static_cast<char>(c);
    return 1;
  }

 private:
  // cannot be assigned
  StreamPrintAdapter& operator=(const StreamPrintAdapter&);

  std::ostream& _os;
};

std::ostream& ArduinoJson::operator<<(std::ostream& os,
                                      const ArduinoJson::JsonVariant& v) {
  StreamPrintAdapter adapter(os);
  v.printTo(adapter);
  return os;
}

std::ostream& ArduinoJson::operator<<(std::ostream& os,
                                      const ArduinoJson::JsonArray& v) {
  StreamPrintAdapter adapter(os);
  v.printTo(adapter);
  return os;
}
