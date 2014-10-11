#pragma once

#include "Arduino/Printable.h"
#include "Internals/JsonNodeIterator.h"
#include "Internals/JsonNode.h"
#include "Internals/IndentedPrint.h"
#include "Internals/JsonNodeWrapper.h"
class JsonArray;
class JsonObject;
class JsonValue;

class JsonContainer : public Printable, public JsonNodeWrapper
{
  // friend JsonValue;
    friend JsonArray;

public:


    JsonContainer() {}

    explicit JsonContainer(JsonNode* node)
        : JsonNodeWrapper(node)
    {
    }

    size_t size() const;
    
    bool operator==(JsonContainer const& other) const;

    size_t printTo(char* buffer, size_t bufferSize) const;
    virtual size_t printTo(Print& print) const;

    size_t prettyPrintTo(char* buffer, size_t bufferSize) const;
    size_t prettyPrintTo(ArduinoJson::Generator::IndentedPrint& print) const;
    size_t prettyPrintTo(Print& print) const;

protected:

    JsonNodeIterator beginChildren() const
    {
        return JsonNodeIterator(_node ? _node->getContainerChild() : 0);
    }

    JsonNodeIterator endChildren() const
    {
        return JsonNodeIterator(0);
    }

    void addChild(JsonNode*);
    void removeChild(JsonNode*);
    JsonNode* createNode();
};

