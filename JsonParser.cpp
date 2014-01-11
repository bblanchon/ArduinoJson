/*
* malloc-free JSON parser for Arduino
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonParser.h"

JsonParserBase::JsonParserBase(jsmntok_t* tokens, int maxTokenCount)
{
	this->maxTokenCount = maxTokenCount;
	this->tokens = tokens;

	jsmn_init(&parser);
}

jsmntok_t* JsonParserBase::parse(char* jsonString)
{	
	if (JSMN_SUCCESS != jsmn_parse(&parser, jsonString, tokens, maxTokenCount))
		return 0;

	// Add null termination to each token
	for (int i = 1; i < parser.toknext; i++)
	{
		jsonString[tokens[i].end] = 0;
	}

	return tokens;
}