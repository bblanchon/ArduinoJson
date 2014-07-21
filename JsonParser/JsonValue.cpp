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

JsonValue::operator double()
{
    return isPrimitive() ? strtod(getText(), 0) : 0;
}

JsonValue::operator long()
{
    return isPrimitive() ? strtol(getText(), 0, 0) : 0;
}

JsonValue::operator char*()
{
    return isString() || isPrimitive() ? getText() : 0;
}

/*
* Returns the token for the value associated with the specified key
*/
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

/*
* Returns the token for the value at the specified index
*/
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