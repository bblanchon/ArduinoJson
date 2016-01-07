// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include "../../include/ArduinoJson/Internals/Comments.hpp"

inline static const char *skipCStyleComment(const char *ptr) {
  ptr += 2;
  for (;;) {
    if (ptr[0] == '\0') return ptr;
    if (ptr[0] == '*' && ptr[1] == '/') return ptr + 2;
    ptr++;
  }
}

inline static const char *skipCppStyleComment(const char *ptr) {
  ptr += 2;
  for (;;) {
    if (ptr[0] == '\0' || ptr[0] == '\n') return ptr;
    ptr++;
  }
}

const char *ArduinoJson::Internals::skipSpacesAndComments(const char *ptr) {
  for (;;) {
    switch (ptr[0]) {
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        ptr++;
        continue;
      case '/':
        switch (ptr[1]) {
          case '*':
            ptr = skipCStyleComment(ptr);
            break;
          case '/':
            ptr = skipCppStyleComment(ptr);
            break;
          default:
            return ptr;
        }
        break;
      default:
        return ptr;
    }
  }
}
