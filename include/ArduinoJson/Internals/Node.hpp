// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include <stddef.h>  // for NULL

namespace ArduinoJson {
namespace Internals {

template <typename T>
struct Node {
  Node() : next(NULL) {}

  Node<T>* next;
  T content;
};
}
}
