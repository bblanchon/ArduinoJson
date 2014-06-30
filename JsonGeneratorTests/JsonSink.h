/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

class JsonSink
{
public:

    virtual void append(char c) = 0;
    virtual void append(const char* s) = 0;

    virtual bool hasRoomFor(int n) = 0;
    virtual void reserveRoom(int n) = 0;
    virtual void releaseRoom(int n) = 0;
};

