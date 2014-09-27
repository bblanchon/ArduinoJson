#pragma once

#include <stddef.h>

class JsonObject;
struct JsonNode;

class JsonBuffer
{
    friend class JsonObject;

public:
//    virtual ~JsonBuffer() = 0;

    JsonObject createObject();

protected:
    virtual /*JsonNode&*/void allocateNode() = 0;
    
    /*JsonNode&*/void createNode();
};

