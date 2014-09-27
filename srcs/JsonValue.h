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
    void operator=(double);
    void operator=(int);

    operator bool();
    operator double();
    operator int();

private:
    //JsonBuffer& _buffer;
    JsonNode* _node;
};

