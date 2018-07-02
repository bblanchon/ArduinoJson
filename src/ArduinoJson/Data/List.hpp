// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/JsonBuffer.hpp"
#include "ListConstIterator.hpp"
#include "ListIterator.hpp"

namespace ArduinoJson {
namespace Internals {

// A singly linked list of T.
// The linked list is composed of ListNode<T>.
// It is derived by JsonArrayData and JsonObjectData
template <typename T>
class List {
 public:
  typedef T value_type;
  typedef ListNode<T> node_type;
  typedef ListIterator<T> iterator;
  typedef ListConstIterator<T> const_iterator;

  explicit List(JsonBuffer *buf) : _buffer(buf), _firstNode(NULL) {}

  // Returns the numbers of elements in the list.
  // For a JsonObjectData, it would return the number of key-value pairs
  size_t size() const {
    size_t nodeCount = 0;
    for (node_type *node = _firstNode; node; node = node->next) nodeCount++;
    return nodeCount;
  }

  iterator add() {
    node_type *newNode = new (_buffer) node_type();

    if (_firstNode) {
      node_type *lastNode = _firstNode;
      while (lastNode->next) lastNode = lastNode->next;
      lastNode->next = newNode;
    } else {
      _firstNode = newNode;
    }

    return iterator(newNode);
  }

  iterator begin() {
    return iterator(_firstNode);
  }
  iterator end() {
    return iterator(NULL);
  }

  const_iterator begin() const {
    return const_iterator(_firstNode);
  }
  const_iterator end() const {
    return const_iterator(NULL);
  }

  void remove(iterator it) {
    node_type *nodeToRemove = it._node;
    if (!nodeToRemove) return;
    if (nodeToRemove == _firstNode) {
      _firstNode = nodeToRemove->next;
    } else {
      for (node_type *node = _firstNode; node; node = node->next)
        if (node->next == nodeToRemove) node->next = nodeToRemove->next;
    }
  }

  JsonBuffer &buffer() const {
    return *_buffer;
  }
  JsonBuffer *_buffer;  // TODO!!

 protected:
  void clear() {
    _firstNode = 0;
  }

 private:
  node_type *_firstNode;
};
}  // namespace Internals
}  // namespace ArduinoJson
