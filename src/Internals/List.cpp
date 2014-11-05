// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../../include/ArduinoJson/Internals/List.hpp"

#include "../../include/ArduinoJson/Internals/PlacementNew.hpp"
#include "../../include/ArduinoJson/JsonPair.hpp"
#include "../../include/ArduinoJson/JsonVariant.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

template <typename T>
int List<T>::size() const {
  int nodeCount = 0;
  for (node_type *node = _firstNode; node; node = node->next) nodeCount++;
  return nodeCount;
}

template <typename T>
typename List<T>::node_type *List<T>::createNode() {
  if (!_buffer) return NULL;
  void *ptr = _buffer->alloc(sizeof(node_type));
  return ptr ? new (ptr) node_type() : NULL;
}

template <typename T>
void List<T>::addNode(node_type *nodeToAdd) {
  if (_firstNode) {
    node_type *lastNode = _firstNode;
    while (lastNode->next) lastNode = lastNode->next;
    lastNode->next = nodeToAdd;
  } else {
    _firstNode = nodeToAdd;
  }
}

template <typename T>
void List<T>::removeNode(node_type *nodeToRemove) {
  if (!nodeToRemove) return;
  if (nodeToRemove == _firstNode) {
    _firstNode = nodeToRemove->next;
  } else {
    for (node_type *node = _firstNode; node; node = node->next)
      if (node->next == nodeToRemove) node->next = nodeToRemove->next;
  }
}

template class List<JsonPair>;
template class List<JsonVariant>;