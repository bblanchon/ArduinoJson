// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/JsonString.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class StringMover {
 public:
  StringMover(char* ptr) : writePtr_(ptr) {}

  void startString() {
    startPtr_ = writePtr_;
  }

  FORCE_INLINE JsonString save() {
    JsonString s = str();
    writePtr_++;
    return s;
  }

  void append(char c) {
    *writePtr_++ = c;
  }

  bool isValid() const {
    return true;
  }

  JsonString str() const {
    writePtr_[0] = 0;  // terminator
    return JsonString(startPtr_, size(), JsonString::Linked);
  }

  size_t size() const {
    return size_t(writePtr_ - startPtr_);
  }

 private:
  char* writePtr_;
  char* startPtr_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
