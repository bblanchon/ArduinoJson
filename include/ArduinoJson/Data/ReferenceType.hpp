// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {

// A type that is meant to be used by reference only (JsonArray and JsonObject)
class ReferenceType {
 protected:
  ReferenceType() {}

 private:
  // copy constructor is private
  ReferenceType(const ReferenceType&);

  // copy operator is private
  ReferenceType& operator=(const ReferenceType&);
};
}
}
