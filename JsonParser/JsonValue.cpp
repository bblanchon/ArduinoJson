/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include <stdlib.h> // for strtol, strtod
#include <string.h> // for strcmp()
#include "JsonArray.h"
#include "JsonHashTable.h"
#include "JsonValue.h"

using namespace ArduinoJson::Parser;

JsonValue::operator bool()
{
    if (tokens == 0 || tokens->type != JSMN_PRIMITIVE) return 0;

    // "true"
    if (json[tokens->start] == 't') return true;

    // "false"
    if (json[tokens->start] == 'f') return false;

    // "null"
    if (json[tokens->start] == 'n') return false;

    // number
    return strtol(json + tokens->start, 0, 0) != 0;
}

JsonValue::operator double()
{
    if (tokens == 0 || tokens->type != JSMN_PRIMITIVE) return 0;

    return strtod(json + tokens->start, 0);
}

JsonValue::operator long()
{
    if (tokens == 0 || tokens->type != JSMN_PRIMITIVE) return 0;

    return strtol(json + tokens->start, 0, 0);
}

JsonValue::operator char*()
{
    if (tokens == 0 || tokens->type != JSMN_PRIMITIVE && tokens->type != JSMN_STRING)
        return 0;

    // add null terminator to the string
    json[tokens->end] = 0;

    return json + tokens->start;
}

JsonValue::operator JsonArray()
{
    return tokens->type != JSMN_ARRAY
        ? JsonArray(*this)
        : JsonArray();
}

JsonValue::operator JsonHashTable()
{
    return tokens->type != JSMN_OBJECT
        ? JsonHashTable(*this)
        : JsonHashTable();
}

/*
* Returns the token for the value associated with the specified key
*/
JsonValue JsonValue::operator [](const char* desiredKey)
{
    // sanity check
    if (!json || !desiredKey || tokens->type != JSMN_OBJECT)
        return JsonValue();

    // skip first token, it's the whole object
    jsmntok_t* currentToken = tokens + 1;

    // scan each keys
    for (int i = 0; i < tokens[0].size / 2; i++)
    {
        // get key token string
        char* key = JsonValue(json, currentToken);

        // compare with desired name
        if (strcmp(desiredKey, key) == 0)
        {
            // return the value token that follows the key token
            return JsonValue(json, currentToken + 1);
        }

        // move forward: key + value + nested tokens
        currentToken += 2 + getNestedTokenCount(currentToken + 1);
    }

    // nothing found, return NULL
    return JsonValue();
}