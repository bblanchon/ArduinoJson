/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

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