/*
* malloc-free JSON parser for Arduino
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonParser.h"

JsonParserBase::JsonParserBase(jsmntok_t* tokens, int maxTokenCount)
{
	this->maxTokenCount = maxTokenCount;
	this->tokens = tokens;
}

jsmntok_t* JsonParserBase::parse(char* jsonString)
{	
	jsmn_parser parser;

	jsmn_init(&parser);

	if (JSMN_SUCCESS != jsmn_parse(&parser, jsonString, tokens, maxTokenCount))
		return 0;

	return tokens;
}