/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "Printable.h"
#include "StringBuilder.h"

class JsonValue : public Printable
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

    JsonValue(Printable& value)
        : implementation(&JsonValue::writeObjectTo)
    {
        content.object = &value;
    }

    virtual size_t writeTo(Print& p) const
    {
        // handmade polymorphism
        return (this->*implementation)(p);
    }
    
private:
    
    union Content
    {
        bool        boolean;
        double      number;
        Printable*  object;
        const char* string;
    };

    Content content;

    size_t(JsonValue::*implementation)(Print& p)const;

    size_t writeBooleanTo(Print& p) const;
    size_t writeNumberTo(Print& p) const;
    size_t writeObjectTo(Print& p) const;
    size_t writeStringTo(Print& p) const;
};