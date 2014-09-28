/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonParserBase.h"
#include "JsonToken.h"

using namespace ArduinoJson::Parser;

JsonValue JsonParserBase::parse(char* json)
{
    jsmn_parser parser;
    jsmn_init(&parser);

    if (JSMN_SUCCESS != jsmn_parse(&parser, json, _tokens, _maxTokens))
        return JsonToken::null();

    return JsonToken(json, _tokens);
}
