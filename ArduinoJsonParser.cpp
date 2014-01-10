/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#include "ArduinoJsonParser.h"

int JsonParserBase::getNestedTokenCounts(int tokenIndex)
{
	int count = 0;

	for (int i = 0; i < tokens[tokenIndex].size; i++)
	{
		count += 1 + getNestedTokenCounts(tokenIndex + 1 + i);
	}

	return count;
}

bool JsonParserBase::parseAndCheckType(char* jsonString, jsmntype_t type)
{
	buffer = jsonString;

	if (JSMN_SUCCESS != jsmn_parse(&parser, jsonString, tokens, maxTokenCount))
		return false;

	if (tokens[0].type != type)
		return false;

	// Add null termination to each token
	for (int i = 1; i < parser.toknext; i++)
	{
		buffer[tokens[i].end] = 0;

		// skip nested objects
		i += getNestedTokenCounts(i);
	}

	return true;
}

char* JsonParserBase::getValueByKey(char* name)
{
	// Scan each keys, every two other token
	// (skip index 0, because it's the whole json object)
	for (int i = 1; i < parser.toknext; i += 2)
	{
		// Get key token string
		char* key = buffer + tokens[i].start;

		// Compare with desired name
		if (strcmp(name, key) == 0)
		{
			return buffer + tokens[i + 1].start;
		}

		// skip nested objects
		i += getNestedTokenCounts(i);
	}
}

char* JsonParserBase::getValueByIndex(int index)
{
	for (int i = 1; i < parser.toknext; i++)
	{
		if (index == 0)
		{
			return buffer + tokens[i].start;
		}

		// skip nested objects
		i += getNestedTokenCounts(i);

		index--;
	}

	return NULL;
}

int JsonParserBase::getArraySize()
{
	int size = 0;

	for (int i = 1; i < parser.toknext; i++)
	{
		// skip nested objects
		i += getNestedTokenCounts(i);

		size++;
	}

	return size;
}
