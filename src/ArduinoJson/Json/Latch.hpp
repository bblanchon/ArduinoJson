// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/assert.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TReader>
class Latch {
 public:
  Latch(TReader reader) : _reader(reader), _loaded(false) {
#if ARDUINOJSON_DEBUG
    _ended = false;
#endif
  }

  void clear() {
    _loaded = false;
  }

  int last() const {
    return __current;
  }

  FORCE_INLINE char current() {
    if (!_loaded) {
      load();
    }
    return __current;
  }

 private:
  void load() {
    ARDUINOJSON_ASSERT(!_ended);
    int c = _reader.read();
#if ARDUINOJSON_DEBUG
    if (c <= 0)
      _ended = true;
#endif
    __current = static_cast<char>(c > 0 ? c : 0);
    _loaded = true;
  }

  TReader _reader;
  char __current;
  bool _loaded;
#if ARDUINOJSON_DEBUG
  bool _ended;
#endif
};

}  // namespace ARDUINOJSON_NAMESPACE
