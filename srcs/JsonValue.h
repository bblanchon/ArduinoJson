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
    void operator=(int);
    
   // operator JsonObject();
    operator int();

private:
    //JsonBuffer& _buffer;
    JsonNode* _node;
};

