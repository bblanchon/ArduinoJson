/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonObjectBase.h"
#include "JsonValue.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonHashTable;

        class JsonArray : public JsonObjectBase
        {
            friend class JsonParserBase;
            friend class JsonValue;

        public:

            JsonArray()	{}

            bool success()
            {
                return JsonObjectBase::success() && tokens->type == JSMN_ARRAY;
            }

            int size()
            {
                return success() ? tokens[0].size : 0;
            }

            JsonValue operator[](int index);

            DEPRECATED int getLength()
            {
                return size();
            }
                      
            DEPRECATED JsonArray getArray(int index)
            {
                return (JsonArray) (*this)[index];
            }

            DEPRECATED bool getBool(int index)
            {
                return (bool) (*this)[index];
            }

            DEPRECATED double getDouble(int index)
            {
                return (double) (*this)[index];
            }

            DEPRECATED JsonHashTable getHashTable(int index);

            DEPRECATED long getLong(int index)
            {
                return (long) (*this)[index];
            }

            DEPRECATED char* getString(int index)
            {
                return (char*) (*this)[index];
            }

        private:

            JsonArray(char* json, jsmntok_t* tokens)
                : JsonObjectBase(json, tokens)
            {

            }
        };
    }
}