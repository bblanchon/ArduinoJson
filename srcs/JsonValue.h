#pragma once

class JsonArray;
class JsonContainer;
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
    void operator=(double x) { set(x, 2); }
    void operator=(int);
    void operator=(const JsonContainer&);
    void operator=(const JsonValue&);
    
    operator bool() const;
    operator const char*() const;
    operator double() const;
    operator long() const;
    operator int() const { return operator long(); }
    operator JsonArray() const;
    operator JsonObject() const;

    void set(double value, int decimals);

private:
    JsonNode* _node;
};