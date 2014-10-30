// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {
namespace Internals {

// A class that is not meant to be copied
class NonCopyable {
 protected:
  NonCopyable() {}

 private:
  // copy constructor is private
  NonCopyable(const NonCopyable&);

  // copy operator is private
  NonCopyable& operator=(const NonCopyable&);
};
}
}
