#pragma once

#include "JsonContainer.h"

class JsonArray : public JsonContainer
{
public:
    JsonArray()
    {
    }

    explicit JsonArray(JsonNode* node)
        : JsonContainer(node)
    {
    }

 //   JsonValue operator[](int index);

   // template<typename T>
   // void add(T value);
};

