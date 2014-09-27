/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonToken.h"

#ifndef ARDUINO_JSON_NO_DEPRECATION_WARNING
#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#endif
#else
#define DEPRECATED
#endif

namespace ArduinoJson
{
    namespace Parser
    {
        // A JSON value
        // Can be converted to string, double, bool, array or object.
        class JsonValue : protected JsonToken
        {
        public:

            // Create a invalid value
            JsonValue()
            {
            }

            // Convert a JsonToken to a JsonValue
            JsonValue(JsonToken token)
                : JsonToken(token)
            {
            }

            // Tell is the JsonValue is valid
            bool success()
            {
                return isValid();
            }

            // Convert the JsonValue to a bool.
            // Returns false if the JsonValue is not a primitve.
            operator bool();

            // Convert the JsonValue to a floating point value.
            // Returns false if the JsonValue is not a number.
            operator double();

            // Convert the JsonValue to a long integer.
            // Returns 0 if the JsonValue is not a number.
            operator long();

            // Convert the JsonValue to a string.
            // Returns 0 if the JsonValue is not a string.
            operator char*();            
            
            // Get the nested value at the specified index.            
            // Returns an invalid JsonValue if the current value is not an array.
            JsonValue operator[](int index);

            // Get the nested value matching the specified index.            
            // Returns an invalid JsonValue if the current value is not an object.
            JsonValue operator[](const char* key);
        };
    }
}
