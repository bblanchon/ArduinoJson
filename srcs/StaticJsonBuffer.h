#pragma once

#include "JsonBuffer.h"
#include "JsonObject.h"

template<int CAPACITY>
class StaticJsonBuffer : public JsonBuffer
{
    friend JsonObject;

public:
    
    explicit StaticJsonBuffer()
        : _size(0)
    {
    }

    virtual ~StaticJsonBuffer() {}

    int capacity()
    {
        return CAPACITY;
    }

    int size()
    {
        return _size;
    }

protected:
    virtual /*JsonNode&*/void allocateNode()
    {
        if (_size < CAPACITY)
            _size++;
    }

private:
    //JsonNode _buffer[CAPACITY];
    int _size;
};