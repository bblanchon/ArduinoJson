// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../../include/ArduinoJson/Internals/QuotedString.hpp"

using namespace ArduinoJson::Internals;

static inline char getSpecialChar(char c) {
  // Optimized for code size on a 8-bit AVR

  const char *p = "\"\"\\\\\bb\ff\nn\rr\tt\0";

  while (p[0] && p[0] != c) {
    p += 2;
  }

  return p[1];
}

static inline size_t printCharTo(char c, Print &p) {
  char specialChar = getSpecialChar(c);

  return specialChar ? p.write('\\') + p.write(specialChar) : p.write(c);
}

size_t QuotedString::printTo(const char *s, Print &p) {
  if (!s) return p.print("null");

  size_t n = p.write('\"');

  while (*s) {
    n += printCharTo(*s++, p);
  }

  return n + p.write('\"');
}

static char unescapeChar(char c) {
  // Optimized for code size on a 8-bit AVR

  const char *p = "b\bf\fn\nr\rt\t";

  for (;;) {
    if (p[0] == '\0') return c;
    if (p[0] == c) return p[1];
    p += 2;
  }
}

static inline bool isQuote(char c) { return c == '\"' || c == '\''; }

char *QuotedString::extractFrom(char *input, char **endPtr) {
  char firstChar = *input;

  if (!isQuote(firstChar)) {
    // must start with a quote
    return NULL;
  }

  char stopChar = firstChar;  // closing quote is the same as opening quote

  char *startPtr = input + 1;  // skip the quote
  char *readPtr = startPtr;
  char *writePtr = startPtr;
  char c;

  for (;;) {
    c = *readPtr++;

    if (c == '\0') {
      // premature ending
      return NULL;
    }

    if (c == stopChar) {
      // closing quote
      break;
    }

    if (c == '\\') {
      // replace char
      c = unescapeChar(*readPtr++);
    }

    *writePtr++ = c;
  }

  // end the string here
  *writePtr = '\0';

  // update end ptr
  *endPtr = readPtr;

  return startPtr;
}
