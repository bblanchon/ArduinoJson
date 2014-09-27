//#include "JsonBuffer.h"
#include "JsonObject.h"
#include "JsonValue.h"
//#include "JsonNode.h"


//JsonValue& JsonObject::operator[](char const* key)
//{
//    addNodeAt(key, innerObject._node);
//    return innerObject;
//}
//
//void JsonObject::addNodeAt(const char* key, JsonNode& node)
//{
//    JsonNode& keyNode = _buffer.createNode();
//    keyNode.becomeKey(key, node);
//    appendChild(keyNode);
//}
//
//void JsonObject::appendChild(JsonNode& newChild)
//{
//    JsonNode* lastChild = _node.asObjectNode.child;
//    while (lastChild->next)
//    {
//        lastChild = lastChild->next;
//    }
//
//    if (lastChild)
//        lastChild->next = &newChild;
//    else
//        _node.asObjectNode.child = &newChild;
//}

JsonValue JsonObject::operator[](char const* key)
{
    _size++;
    return JsonValue();
}