// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Configuration.hpp"

#if ARDUINOJSON_ENABLE_STD_STREAM

#include <ostream>

namespace ArduinoJson {
namespace Internals {

class StreamWriter {
 public:
  explicit StreamWriter(std::ostream& os) : _os(os) {}

  size_t write(uint8_t c) {
    _os << c;
    return 1;
  }

  size_t write(const uint8_t* s, size_t n) {
    _os.write(reinterpret_cast<const char*>(s),
              static_cast<std::streamsize>(n));
    return n;
  }

 private:
  // cannot be assigned
  StreamWriter& operator=(const StreamWriter&);

  std::ostream& _os;
};
}  // namespace Internals
}  // namespace ArduinoJson

#endif  // ARDUINOJSON_ENABLE_STD_STREAM
