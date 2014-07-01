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

    size_t writeTo(JsonSink& sink)
    {
        // handmade polymorphism
        return (this->*implementation)(sink);
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

    size_t (JsonValue::*implementation)(JsonSink& sb);

    size_t writeBooleanTo(JsonSink& sb);
    size_t writeNumberTo(JsonSink& sb);
    size_t writeObjectTo(JsonSink& sb);
    size_t writeStringTo(JsonSink& sb);
};