/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonSink.h"

class StringBuilder : public JsonSink
{
public:
    StringBuilder(char* buf, int size)
        : buffer(buf), capacity(size-1), length(0)
    {
        buffer[0] = 0;
    }

    virtual void append(char c);
    virtual void append(const char* s);

    virtual bool hasRoomFor(int n)
    {
        return capacity - length >= n;
    }

    virtual void reserveRoom(int n)
    {
        capacity -= n;
    }

    virtual void releaseRoom(int n)
    {
        capacity += n;
    }

private:
    char* buffer;
    int capacity;
    int length;
};

