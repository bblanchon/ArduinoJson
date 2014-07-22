/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include <stdlib.h> // for strtol, strtod
#include <string.h> // for strcmp()
#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonValue.h"

using namespace ArduinoJson::Parser;

// Convert the JsonValue to a bool.
// Returns false if the JsonValue is not a primitve.
JsonValue::operator bool()
{
    if (!isPrimitive()) return 0;

    char *text = getText();

    // "true"
    if (text[0] == 't') return true;

    // "false"
    if (text[0] == 'f') return false;

    // "null"
    if (text[0] == 'n') return false;

    // number
    return strtol(text, 0, 0) != 0;
}

// Convert the JsonValue to a floating point value.
// Returns false if the JsonValue is not a number.
JsonValue::operator double()
{
    return isPrimitive() ? strtod(getText(), 0) : 0;
}

// Convert the JsonValue to a floating point value.
// Returns false if the JsonValue is not a number.
JsonValue::operator long()
{
    return isPrimitive() ? strtol(getText(), 0, 0) : 0;
}

// Convert the JsonValue to a string.
// Returns 0 if the JsonValue is not a string.
JsonValue::operator char*()
{
    return isString() || isPrimitive() ? getText() : 0;
}

// Get the nested value at the specified index.            
// Returns an invalid JsonValue if the current value is not an array.
JsonValue JsonValue::operator[](int index)
{
    // sanity check
    if (index < 0 || !isArray() || index >= childrenCount())
        return null();

    // skip first token, it's the whole object
    JsonToken runningToken = firstChild();

    // skip all tokens before the specified index
    for (int i = 0; i < index; i++)
    {
        // move forward: current + nested tokens
        runningToken = runningToken.nextSibling();
    }

    return runningToken;
}

// Get the nested value matching the specified index.            
// Returns an invalid JsonValue if the current value is not an object.
JsonValue JsonValue::operator[](const char* desiredKey)
{
    // sanity check
    if (desiredKey == 0 || !isObject())
        return null();

    // skip first token, it's the whole object
    JsonToken runningToken = firstChild();

    // scan each keys
    for (int i = 0; i < childrenCount() / 2; i++)
    {
        // get 'key' token string
        char* key = runningToken.getText();

        // move to the 'value' token
        runningToken = runningToken.nextSibling();

        // compare with desired name
        if (strcmp(desiredKey, key) == 0)
        {
            // return the value token that follows the key token
            return runningToken;
        }

        // skip nested tokens
        runningToken = runningToken.nextSibling();
    }

    // nothing found, return NULL
    return null();
}
