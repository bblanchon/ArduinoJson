#pragma once

#include "JsonBuffer.h"
#include "JsonObject.h"

template<int CAPACITY>
class StaticJsonBuffer //: public JsonBuffer
{
public:
    
    explicit StaticJsonBuffer()
        : _size(0)
    {
    }

    virtual ~StaticJsonBuffer() {}

    JsonObject createObject()
    {
        if (_size < CAPACITY)
            _size++;

        return JsonObject();
    }

    int capacity()
    {
        return CAPACITY;
    }

    int size()
    {
        return _size;
    }

protected:
    //virtual JsonNode& allocateNode();

private:
    //JsonNode _buffer[CAPACITY];
    int _size;
};