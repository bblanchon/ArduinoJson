// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include <stddef.h>  // for NULL

namespace ArduinoJson {
namespace Internals {

// A node for a singly-linked list.
// Used by List<T> and its iterators.
template <typename T>
struct ListNode {
  ListNode() : next(NULL) {}

  ListNode<T>* next;
  T content;
};
}
}
