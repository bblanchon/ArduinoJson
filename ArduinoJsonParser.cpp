/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#include "ArduinoJsonParser.h"

bool JsonParserBase::parseTokens(char* jsonString)
{
	buffer = jsonString;

	if (JSMN_SUCCESS != jsmn_parse(&parser, jsonString, tokens, tokenCount))
		return false;

	// Add null termination to each token
	for (int i = 0; i < tokenCount; i++)
	{
		buffer[tokens[i].end] = 0;
	}

	return true;
}

char* JsonParserBase::getValueByKey(char* name)
{
	// Scan each keys, every two other token
	// (skip index 0, because it's the whole json object)
	for (int i = 1; i < tokenCount; i += 2)
	{
		// Early break if we reach the last token
		if (i >= parser.toknext) break;

		// Get key token string
		char* key = buffer + tokens[i].start;

		// Compare with desired name
		if (strcmp(name, key) == 0)
		{
			return buffer + tokens[i + 1].start;
		}
	}
}

