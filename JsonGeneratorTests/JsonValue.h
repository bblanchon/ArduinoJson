/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "StringBuilder.h"

class JsonObjectBase;

class JsonValue
{    
public:

    JsonValue()
    {
    }

    JsonValue(const char* value)
        : implementation(&JsonValue::writeStringTo)
    {
        content.string = value;
    }

    JsonValue(double value)
        : implementation(&JsonValue::writeNumberTo)
    {
        content.number = value;
    }

    JsonValue(bool value)
        : implementation(&JsonValue::writeBooleanTo)
    {
        content.boolean = value;
    }

    JsonValue(JsonObjectBase& value)
        : implementation(&JsonValue::writeObjectTo)
    {
        content.object = &value;
    }

    size_t writeTo(Print& p)
    {
        // handmade polymorphism
        return (this->*implementation)(p);
    }
    
private:
    
    union Content
    {
        bool            boolean;
        double          number;
        JsonObjectBase* object;
        const char*     string;
    };

    Content content;

    size_t(JsonValue::*implementation)(Print& p);

    size_t writeBooleanTo(Print& p);
    size_t writeNumberTo(Print& p);
    size_t writeObjectTo(Print& p);
    size_t writeStringTo(Print& p);
};