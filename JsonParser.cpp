/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#include "JsonParser.h"

bool JsonParserBase::parse(char* jsonString)
{
	buffer = jsonString;

	if (JSMN_SUCCESS != jsmn_parse(&parser, jsonString, tokens, maxTokenCount))
		return false;

	// Add null termination to each token
	for (int i = 1; i < parser.toknext; i++)
	{
		buffer[tokens[i].end] = 0;
	}

	return true;
}