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
        // A pointer to a JSON token
        class JsonToken
        {
        public:

            // Create a "null" pointer
            JsonToken()
                : token(0)
            {
            }

            // Create a pointer to the specified JSON token
            JsonToken(char* json, jsmntok_t* token)
                : json(json), token(token)
            {
            }

            // Get content of the JSON token
            char* getText();

            // Get the number of children tokens
            int childrenCount()
            {
                return token->size;
            }

            // Get a pointer to the first child of the current token
            JsonToken firstChild() const
            {
                return JsonToken(json, token + 1);
            }

            // Get a pointer to the next sibling token (ie skiping the children tokens)
            JsonToken nextSibling() const;

            // Test equality
            bool operator!=(const JsonToken& other) const
            {
                return token != other.token;
            }

            // Tell if the pointer is "null"
            bool isValid()
            {
                return token != 0;
            }

            // Tell if the JSON token is a JSON object
            bool isObject()
            {
                return token != 0 && token->type == JSMN_OBJECT;
            }

            // Tell if the JSON token is a JSON array
            bool isArray()
            {
                return token != 0 && token->type == JSMN_ARRAY;
            }

            // Tell if the JSON token is a primitive
            bool isPrimitive()
            {
                return token != 0 && token->type == JSMN_PRIMITIVE;
            }

            // Tell if the JSON token is a string
            bool isString()
            {
                return token != 0 && token->type == JSMN_STRING;
            }

            // Explicit wait to create a "null" JsonToken
            static JsonToken null()
            {
                return JsonToken();
            }

        private:
            char* json;
            jsmntok_t* token;
            
            static char unescapeChar(char c);
            static void unescapeString(char* s);
        };
    }
}