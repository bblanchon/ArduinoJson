// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Serialization/WriterSelector.hpp>

#include <ostream>

namespace ARDUINOJSON_NAMESPACE {

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

template <typename TDestination>
struct WriterSelector<
    TDestination,
    typename enable_if<is_base_of<std::ostream, TDestination>::value>::type> {
  typedef StreamWriter writer_type;
};
}  // namespace ARDUINOJSON_NAMESPACE
