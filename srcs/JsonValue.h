#pragma once

struct JsonNode;

class JsonValue
{
public:
    
    explicit JsonValue()
        : _node(0)
    {
    }

    explicit JsonValue(JsonNode* node)
        : _node(node)
    {
    }

    void operator=(bool);
    void operator=(const char*);
    void operator=(double);
    void operator=(int);
    void operator=(const JsonObject&);
    
    operator bool() const;
    operator const char*() const;
    operator double() const;
    operator int() const;
    operator JsonObject() const;

private:
    JsonNode* _node;

    JsonNode* getActualNode() const;
};

