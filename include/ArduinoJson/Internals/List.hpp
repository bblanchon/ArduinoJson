// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "ListIterator.hpp"
#include "ListConstIterator.hpp"
#include "../JsonBuffer.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename T>
class List {
 public:
  typedef T value_type;
  typedef ListNode<T> node_type;
  typedef ListIterator<T> iterator;
  typedef ListConstIterator<T> const_iterator;

  List(JsonBuffer *buffer) : _buffer(buffer), _firstNode(NULL) {}

  bool success() const { return _buffer != NULL; }
  int size() const;

  iterator begin() { return iterator(_firstNode); }
  iterator end() { return iterator(NULL); }

  const_iterator begin() const { return const_iterator(_firstNode); }
  const_iterator end() const { return const_iterator(NULL); }

 protected:
  node_type *createNode();
  void addNode(node_type *node);
  void removeNode(node_type *nodeToRemove);

  JsonBuffer *_buffer;
  node_type *_firstNode;
};
}
}
