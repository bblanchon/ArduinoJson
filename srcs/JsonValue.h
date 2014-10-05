#pragma once

class JsonObject;
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
    void operator=(const JsonValue&);
    
    operator bool() const;
    operator const char*() const;
    operator double() const;
    operator int() const;
    operator JsonObject() const;

    void set(double value, int decimals);

private:
    JsonNode* _node;

    void setAsProxyTo(JsonNode*);
    JsonNode* getActualNode() const;
};