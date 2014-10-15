#pragma once

#include "JsonNode.h"

class JsonNodeIterator
{
public:

    explicit JsonNodeIterator(JsonNode* node)
        : node(node)
    {
    }

    bool operator!= (const JsonNodeIterator& other) const
    {
        return node != other.node;
    }

    void operator++()
    {
        node = node->next;
    }

    JsonNode* operator*() const
    {
        return node;
    }

    JsonNode* operator->() const
    {
        return node;
    }

private:
    JsonNode* node;
};

