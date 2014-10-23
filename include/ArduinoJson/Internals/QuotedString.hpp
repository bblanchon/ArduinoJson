// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../Arduino/Print.hpp"

namespace ArduinoJson {
namespace Internals {
class QuotedString {
 public:
  static size_t printTo(const char *, Print *);

  static char *extractFrom(char *input, char **end);
};
}
}
