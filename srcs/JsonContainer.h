#pragma once

#include "Arduino\Printable.h"

struct JsonNode;

class JsonContainer : public Printable
{
public:
    JsonContainer()
        : _node(0)
    {
    }

    JsonContainer(JsonNode* node)
        : _node(node)
    {
    }

    size_t printTo(char* buffer, size_t bufferSize) const;
    virtual size_t printTo(Print& print) const;

protected:
    JsonNode* _node;
};

