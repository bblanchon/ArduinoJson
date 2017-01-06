// Copyright Benoit Blanchon 2014-2017
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
    switch (input.current()) {
      // spaces
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        input.move();
        continue;

      // comments
      case '/':
        switch (input.next()) {
          // C-style block comment
          case '*':
            input.move();  // skip '/'
            input.move();  // skip '*'
            for (;;) {
              switch (input.current()) {
                case '\0':
                  return;
                case '*':
                  input.move();  // skip '*'
                  if (input.current() == '/') {
                    input.move();  // skip '/'
                    return;
                  }
                  break;
                default:
                  input.move();
              }
            }
            break;

          // C++-style line comment
          case '/':
            input.move();  // skip '/'
            for (;;) {
              switch (input.current()) {
                case '\0':
                  return;
                case '\n':
                  input.move();
                  return;
                default:
                  input.move();
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
