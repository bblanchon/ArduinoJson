/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonValue.h"
#include "Print.h"
#include "Printable.h"

class JsonObjectBase : public Printable
{
public:

    size_t writeTo(char* buffer, size_t bufferSize)
    {
        StringBuilder sb(buffer, bufferSize);
        return writeTo(sb);
    }

    virtual size_t writeTo(Print& p) const = 0;
};

