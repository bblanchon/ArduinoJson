/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include <stdlib.h> // for strtol, strtod
#include "JsonArray.h"
#include "JsonHashTable.h"
#include "JsonValue.h"

using namespace ArduinoJson::Parser;

JsonValue::operator bool()
{
    if (token == 0 || token->type != JSMN_PRIMITIVE) return 0;

    // "true"
    if (json[token->start] == 't') return true;

    // "false"
    if (json[token->start] == 'f') return false;

    // "null"
    if (json[token->start] == 'n') return false;

    // number
    return strtol(json + token->start, 0, 0) != 0;
}

JsonValue::operator double()
{
    if (token == 0 || token->type != JSMN_PRIMITIVE) return 0;

    return strtod(json + token->start, 0);
}

JsonValue::operator long()
{
    if (token == 0 || token->type != JSMN_PRIMITIVE) return 0;

    return strtol(json + token->start, 0, 0);
}

JsonValue::operator char*()
{
    if (token == 0 || token->type != JSMN_PRIMITIVE && token->type != JSMN_STRING)
        return 0;

    // add null terminator to the string
    json[token->end] = 0;

    return json + token->start;
}

JsonValue::operator JsonArray()
{
    return JsonArray(json, token);
}

JsonValue::operator JsonHashTable()
{
    return JsonHashTable(json, token);
}