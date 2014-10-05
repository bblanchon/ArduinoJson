#include "JsonContainer.h"

#include "JsonBuffer.h"
#include "Internals/JsonWriter.h"
#include "Internals/StringBuilder.h"

size_t JsonContainer::printTo(char* buffer, size_t bufferSize) const
{
    ArduinoJson::Internals::StringBuilder sb(buffer, bufferSize);
    return printTo(sb);
}

size_t JsonContainer::printTo(Print& p) const
{
    JsonWriter writer(p);
    _node->writeTo(writer);
    return writer.bytesWritten();
}

JsonNode* JsonContainer::createNode(JsonNodeType type)
{
    JsonBuffer* buffer = _node->content.asContainer.buffer;
    return buffer->createNode(type);
}

bool JsonContainer::checkNodeType(JsonNodeType expectedType)
{
    return _node && _node->type == expectedType;
}

bool JsonContainer::operator==(const JsonContainer & other) const
{
    return _node == other._node;
}

void JsonContainer::addChild(JsonNode* newChild)
{
    JsonNode* lastChild = _node->content.asContainer.child;

    if (!lastChild)
    {
        _node->content.asContainer.child = newChild = newChild;
        return;
    }

    while (lastChild->next)
        lastChild = lastChild->next;

    lastChild->next = newChild;
}

void JsonContainer::removeChildAfter(JsonNode* child, JsonNode* previous)
{
    if (previous)
        previous->next = child->next;
    else
        _node->content.asContainer.child = child->next;
}

size_t JsonContainer::size() const
{
    int size = 0;

    for (JsonNodeIterator it = beginChildren(); it != endChildren(); ++it)
    {
        size++;
    }

    return size;
}

