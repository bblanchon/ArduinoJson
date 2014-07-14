/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonParserBase.h"

using namespace ArduinoJson::Parser;

jsmntok_t* JsonParserBase::parse(char* json)
{
    jsmn_parser parser;
    jsmn_init(&parser);

    if (JSMN_SUCCESS != jsmn_parse(&parser, json, tokens, maxTokens))
        return 0;

    return tokens;
}
