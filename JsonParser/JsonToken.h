/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "jsmn.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonToken
        {
        public:

            JsonToken()
                : token(0)
            {

            }

            JsonToken(char* json, jsmntok_t* token)
                : json(json),  token(token)
            {

            }

            char* getText()
            {
                json[token->end] = 0;
                return json + token->start;
            }

            JsonToken firstChild() const
            {
                return JsonToken(json, token + 1);
            }

            JsonToken nextSibling() const;

            bool operator!= (const JsonToken& other) const
            {
                return token != other.token;
            }

            static JsonToken null()
            {
                return JsonToken(0, 0);
            }

            bool isValid()
            {
                return token != 0;
            }

            bool isObject()
            {
                return token != 0 && token->type == JSMN_OBJECT;
            }

            bool isArray()
            {
                return token != 0 && token->type == JSMN_ARRAY;
            }

            bool isPrimitive()
            {
                return token != 0 && token->type == JSMN_PRIMITIVE;
            }

            bool isString()
            {
                return token != 0 && token->type == JSMN_STRING;
            }

            int childrenCount()
            {
                return token->size;
            }

        private:
            char* json;
            jsmntok_t* token;
        };
    }
}

