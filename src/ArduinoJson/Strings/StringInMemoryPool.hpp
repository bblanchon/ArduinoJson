// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "ZeroTerminatedRamStringConst.hpp"

namespace ARDUINOJSON_NAMESPACE {

class StringInMemoryPool : public ZeroTerminatedRamStringConst {
 public:
  StringInMemoryPool(const char* str = 0) : ZeroTerminatedRamStringConst(str) {}
};

}  // namespace ARDUINOJSON_NAMESPACE
