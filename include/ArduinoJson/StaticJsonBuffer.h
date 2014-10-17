#pragma once

#include "JsonBuffer.h"
#include "JsonObject.h"

template<int CAPACITY>
class StaticJsonBuffer : public JsonBuffer
{
    friend class JsonObject;

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
    virtual void* allocateNode()
    {
        if (_size >= CAPACITY) return 0;

        return &_buffer[_size++];
    }

private:
    JsonNode _buffer[CAPACITY];
    int _size;
};

