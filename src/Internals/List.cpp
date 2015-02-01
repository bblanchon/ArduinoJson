// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../../include/ArduinoJson/Internals/List.hpp"

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
