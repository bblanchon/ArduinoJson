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
Node<T> *List<T>::createNode() {
  if (!_buffer) return NULL;
  void *ptr = _buffer->alloc(sizeof(node_type));
  return ptr ? new (ptr) node_type() : NULL;
}

template class List<JsonPair>;
template class List<JsonVariant>;