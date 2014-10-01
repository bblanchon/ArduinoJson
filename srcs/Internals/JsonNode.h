#pragma once

class JsonBuffer;

enum JsonNodeType
{
    JSON_UNDEFINED,
    JSON_PROXY,
    JSON_NULL,
    JSON_ARRAY,
    JSON_OBJECT,
    JSON_KEY,
    JSON_BOOLEAN,
    JSON_STRING,
    JSON_INTEGER,
    JSON_DOUBLE_0_DECIMALS,
    JSON_DOUBLE_1_DECIMAL,
    JSON_DOUBLE_2_DECIMALS,
    // etc.
};

struct JsonNode
{
    JsonNode* next;
    JsonNodeType type;

    union
    {
        bool asBoolean;
        double asDouble;
        long asInteger;
        const char* asString;

        struct 
        {
            const char* key;
            JsonNode* value;
        } asKey;

        struct
        {
            JsonNode* child;
            JsonBuffer* buffer;
        } asObject;

        struct
        {
            JsonNode* target;
        } asProxy;

    } content;
};