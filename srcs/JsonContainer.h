#pragma once

#include "Arduino/Printable.h"
#include "Internals/JsonNodeIterator.h"
#include "Internals/JsonNode.h"

struct JsonNode;
class JsonValue;

class JsonContainer : public Printable
{
    friend JsonValue;

public:
    JsonContainer()
        : _node(0)
    {
    }

    JsonContainer(JsonNode* node)
        : _node(node)
    {
    }

    size_t size() const;
    
    bool operator==(JsonContainer const& other) const;

    size_t printTo(char* buffer, size_t bufferSize) const;
    virtual size_t printTo(Print& print) const;

protected:

    JsonNodeIterator beginChildren() const
    {
        return JsonNodeIterator(_node ? _node->content.asContainer.child : 0);
    }

    JsonNodeIterator endChildren() const
    {
        return JsonNodeIterator(0);
    }

    void insertChildAfter(JsonNode* newChild, JsonNode* insertAfterMe);
    void removeChildAfter(JsonNode* child, JsonNode* previous);
    JsonNode* createNode(JsonNodeType type);

    bool checkNodeType(JsonNodeType expectedType);

private:
    JsonNode* _node;
};

