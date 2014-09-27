#pragma once

//class JsonBuffer;
//class JsonNode;
//class JsonObject;

class JsonValue
{
public:

    JsonValue(JsonNode* node)
        : _node(node)
    {
    }

   // void operator=(const JsonObject& object);
    void operator=(double);
    void operator=(int);
    
   // operator JsonObject();
    operator double();
    operator int();

private:
    //JsonBuffer& _buffer;
    JsonNode* _node;
};

