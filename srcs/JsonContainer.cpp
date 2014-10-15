#include "ArduinoJson/JsonContainer.h"

#include "ArduinoJson/JsonBuffer.h"
#include "ArduinoJson/Internals/StringBuilder.h"
#include "ArduinoJson/Internals/CompactJsonWriter.h"
#include "ArduinoJson/Internals/PrettyJsonWriter.h"

using namespace ArduinoJson::Internals;

size_t JsonContainer::printTo(char* buffer, size_t bufferSize) const
{
    StringBuilder sb(buffer, bufferSize);
    return printTo(sb);
}

size_t JsonContainer::printTo(Print& p) const
{
    CompactJsonWriter writer(&p);
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
    PrettyJsonWriter writer(&p);
    _node->writeTo(writer);
    return writer.bytesWritten();
}

size_t JsonContainer::prettyPrintTo(Print& print) const
{
    IndentedPrint indentedPrint = IndentedPrint(print);
    return prettyPrintTo(indentedPrint);
}

JsonNode* JsonContainer::createNode()
{
    if (!_node) return 0;

    JsonBuffer* buffer = _node->getContainerBuffer();
    if (!buffer) return 0;

    return buffer->createNode();
}

bool JsonContainer::operator==(const JsonContainer & other) const
{
    if (_node == other._node) return true;
    if (!_node || !other._node) return false;
    return _node->getProxyTarget() == other._node->getProxyTarget();
}

void JsonContainer::addChild(JsonNode* childToAdd)
{
    if (_node)
        _node->addChild(childToAdd);
}

void JsonContainer::removeChild(JsonNode* childToRemove)
{
    if (_node)
        _node->removeChild(childToRemove);
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

