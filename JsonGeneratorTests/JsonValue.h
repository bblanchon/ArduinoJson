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

    void writeTo(StringBuilder& sb)
    {
        // handmade polymorphism
       (this->*implementation)(sb);
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

    void (JsonValue::*implementation)(StringBuilder& sb);

    void writeBooleanTo(StringBuilder& sb);
    void writeNumberTo(StringBuilder& sb);
    void writeObjectTo(StringBuilder& sb);
    void writeStringTo(StringBuilder& sb);
};