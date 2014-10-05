#pragma once

class Print;
struct JsonNode;

class JsonNodeSerializer
{
public:
    explicit JsonNodeSerializer(Print& sink)
        : _sink(sink)
    {
    }

    size_t serialize(const JsonNode* node);

private:
    Print& _sink;

    size_t serializeArray(const JsonNode* node);
    size_t serializeObject(const JsonNode* node);
    size_t serializeKeyValue(const JsonNode* node);
};

