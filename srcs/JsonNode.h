#pragma once

enum JsonNodeType
{
    JSON_UNDEFINED,
    JSON_NULL,
    JSON_ARRAY,
    JSON_OBJECT,
    JSON_KEY,
    JSON_TRUE,
    JSON_FALSE,
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
     //   int asInteger;

        struct 
        {
            const char* key;
            JsonNode* value;
        } asKey;

        struct
        {
            JsonNode* child;
        } asObjectNode;

    } content;
};