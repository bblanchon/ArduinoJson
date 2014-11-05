// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename T>
struct Node {
  Node() : next(NULL) {}

  T content;
  Node<T>* next;
};
}
}
