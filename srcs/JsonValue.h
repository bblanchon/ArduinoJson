#pragma once

//class JsonBuffer;
//class JsonNode;
//class JsonObject;

class JsonValue
{
public:
    
    explicit JsonValue(JsonNode* node)
        : _node(node)
    {
    }

    void operator=(bool);
    void operator=(const char*);
    void operator=(double);
    void operator=(int);

    operator bool() const;
    operator const char*() const;
    operator double() const;
    operator int() const;

private:
    //JsonBuffer& _buffer;
    JsonNode* _node;
};

