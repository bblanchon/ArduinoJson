#include "JsonContainer.h"

#include "JsonBuffer.h"
#include "Internals/StringBuilder.h"
#include "Internals/CompactJsonWriter.h"
#include "Internals/PrettyJsonWriter.h"

using namespace ArduinoJson::Internals;

size_t JsonContainer::printTo(char* buffer, size_t bufferSize) const
{
    StringBuilder sb(buffer, bufferSize);
    return printTo(sb);
}

size_t JsonContainer::printTo(Print& p) const
{
    CompactJsonWriter writer(p);
    _node->writeTo(writer);
    return writer.bytesWritten();
}

size_t JsonContainer::prettyPrintTo(char* buffer, size_t bufferSize) const
{
    StringBuilder sb(buffer, bufferSize);
    return prettyPrintTo(sb);
}

size_t JsonContainer::prettyPrintTo(IndentedPrint& p) const
{
    PrettyJsonWriter writer(p);
    _node->writeTo(writer);
    return writer.bytesWritten();
}

JsonNode* JsonContainer::createNode(JsonNodeType type)
{
    if (!_node) return 0;

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

