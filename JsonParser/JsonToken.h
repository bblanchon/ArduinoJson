/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "jsmn.h"

namespace ArduinoJson
{
    namespace Internal
    {
        class JsonToken
        {
        public:

            JsonToken(jsmntok_t* token)
                :token(token)
            {

            }

            char* getText(char* json)
            {
                json[token->end] = 0;
                return json + token->start;
            }

            JsonToken operator+ (int n)
            {
                return JsonToken(token + n);
            }

            void operator+= (int n)
            {
                token += n;
            }

            bool operator!= (const JsonToken& other)
            {
                return token != other.token;
            }

            int size()
            {
                return token->size;
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

            static JsonToken null()
            {
                return JsonToken(0);
            }

            int nestedTokenCount() const;

        private:
            jsmntok_t* token;
        };
    }
}

