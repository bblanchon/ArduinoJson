// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../Arduino/Print.hpp"

namespace ArduinoJson {
namespace Internals {

// An helper class to print and extract doubly-quoted strings
class QuotedString {
 public:
  // Writes a doubly-quote string to a Print implementation.
  // It adds the double quotes (") at the beginning and the end of the string.
  // It escapes the special characters as required by the JSON specifications.
  static size_t printTo(const char *, Print &);

  // Reads a doubly-quoted string from a buffer.
  // It removes the double quotes (").
  // It unescapes the special character as required by the JSON specification,
  // with the exception of the Unicode characters (\u0000).
  static char *extractFrom(char *input, char **end);
};
}
}
