// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/assert.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

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
    return _current;
  }

  FORCE_INLINE char current() {
    if (!_loaded) {
      load();
    }
    return _current;
  }

 private:
  void load() {
    ARDUINOJSON_ASSERT(!_ended);
    int c = _reader.read();
#if ARDUINOJSON_DEBUG
    if (c <= 0)
      _ended = true;
#endif
    _current = static_cast<char>(c > 0 ? c : 0);
    _loaded = true;
  }

  TReader _reader;
  char _current;  // NOLINT(clang-analyzer-optin.cplusplus.UninitializedObject)
                  // Not initialized in constructor (+10 bytes on AVR)
  bool _loaded;
#if ARDUINOJSON_DEBUG
  bool _ended;
#endif
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
