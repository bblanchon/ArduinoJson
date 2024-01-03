// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/assert.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TReader>
class Latch {
 public:
  Latch(TReader reader) : reader_(reader), loaded_(false) {
#if ARDUINOJSON_DEBUG
    ended_ = false;
#endif
  }

  void clear() {
    loaded_ = false;
  }

  int last() const {
    return current_;
  }

  FORCE_INLINE char current() {
    if (!loaded_) {
      load();
    }
    return current_;
  }

 private:
  void load() {
    ARDUINOJSON_ASSERT(!ended_);
    int c = reader_.read();
#if ARDUINOJSON_DEBUG
    if (c <= 0)
      ended_ = true;
#endif
    current_ = static_cast<char>(c > 0 ? c : 0);
    loaded_ = true;
  }

  TReader reader_;
  char current_;  // NOLINT(clang-analyzer-optin.cplusplus.UninitializedObject)
                  // Not initialized in constructor (+10 bytes on AVR)
  bool loaded_;
#if ARDUINOJSON_DEBUG
  bool ended_;
#endif
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
