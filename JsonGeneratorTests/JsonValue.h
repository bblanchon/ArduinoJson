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

    void writeTo(JsonSink& sink)
    {
        // handmade polymorphism
        (this->*implementation)(sink);
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

    void (JsonValue::*implementation)(JsonSink& sb);

    void writeBooleanTo(JsonSink& sb);
    void writeNumberTo(JsonSink& sb);
    void writeObjectTo(JsonSink& sb);
    void writeStringTo(JsonSink& sb);
};