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
};

