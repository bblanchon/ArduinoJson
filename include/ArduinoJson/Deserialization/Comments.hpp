// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {
template <typename TInput>
void skipSpacesAndComments(TInput& input) {
  for (;;) {
    switch (input.peek()) {
      // spaces
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        input.skip();
        continue;

      // comments
      case '/':
        switch (input.peekNext()) {
          // C-style block comment
          case '*':
            input.skip();  // skip '/'
            input.skip();  // skip '*'
            for (;;) {
              switch (input.peek()) {
                case '\0':
                  return;
                case '*':
                  input.skip();  // skip '*'
                  if (input.peek() == '/') {
                    input.skip();  // skip '/'
                    return;
                  }
                  break;
                default:
                  input.skip();
              }
            }
            break;

          // C++-style line comment
          case '/':
            input.skip();  // skip '/'
            for (;;) {
              switch (input.peek()) {
                case '\0':
                  return;
                case '\n':
                  input.skip();
                  return;
                default:
                  input.skip();
              }
            }
            return;

          // not a comment, just a '/'
          default:
            return;
        }
        break;

      default:
        return;
    }
  }
}
}
}
