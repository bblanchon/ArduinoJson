/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonObjectBase.h"

template<int N>
class JsonHashTable : public JsonObjectBase
{
public:

    using JsonObjectBase::writeTo;

private:

   /* struct KeyValuePair
    {
        const char* key;
        ObjectContainer value;
    };*/

    virtual void writeTo(StringBuilder& sb)
    {
        sb.append("{");

        /*for (int i = 0; i < itemCount; i++)
        {
            if (i>0) sb.append(",");
            writeObjectTo(items[i], sb);
        }*/

        sb.append("}");
    }
};

