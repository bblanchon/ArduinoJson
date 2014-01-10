/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#include "ArduinoJsonParser.h"

bool JsonParserBase::parseAndCheckType(char* jsonString, jsmntype_t type)
{
	buffer = jsonString;

	if (JSMN_SUCCESS != jsmn_parse(&parser, jsonString, tokens, maxTokenCount))
		return false;

	if (tokens[0].type != type)
		return false;

	// Add null termination to each token
	for (int i = 0; i < maxTokenCount; i++)
	{
		buffer[tokens[i].end] = 0;
	}

	return true;
}

char* JsonParserBase::getValueByKey(char* name)
{
	// Scan each keys, every two other token
	// (skip index 0, because it's the whole json object)
	for (int i = 1; i < maxTokenCount; i += 2)
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

char* JsonParserBase::getValueByIndex(int index)
{
	if (index < 0 || index >= parser.toknext)
		return NULL;

	return buffer + tokens[index + 1].start;
}
