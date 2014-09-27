#pragma once

#include "JsonBuffer.h"

template<int CAPACITY>
class StaticJsonBuffer //: public JsonBuffer
{
public:
    
    explicit StaticJsonBuffer()
        : _size(0)
    {
    }

    virtual ~StaticJsonBuffer() {}

    /*JsonObject*/
    void createObject()
    {
        if (_size < CAPACITY)
            _size++;
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