// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include "../../include/ArduinoJson/Internals/List.hpp"

#include "../../include/ArduinoJson/JsonPair.hpp"
#include "../../include/ArduinoJson/JsonVariant.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

template <typename T>
size_t List<T>::size() const {
  size_t nodeCount = 0;
  for (node_type *node = _firstNode; node; node = node->next) nodeCount++;
  return nodeCount;
}

template <typename T>
typename List<T>::node_type *List<T>::addNewNode() {
  node_type *newNode = new (_buffer) node_type();

  if (_firstNode) {
    node_type *lastNode = _firstNode;
    while (lastNode->next) lastNode = lastNode->next;
    lastNode->next = newNode;
  } else {
    _firstNode = newNode;
  }

  return newNode;
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

template class ArduinoJson::Internals::List<JsonPair>;
template class ArduinoJson::Internals::List<JsonVariant>;
