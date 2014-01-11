/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#include "ArduinoJsonParser.h"

int JsonObjectBase::getNestedTokenCounts(int tokenIndex)
{
	int count = 0;

	for (int i = 0; i < tokens[tokenIndex].size; i++)
	{
		count += 1 + getNestedTokenCounts(tokenIndex + 1 + i);
	}

	return count;
}

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

char* JsonHashTable::getString(char* name)
{	
	// skip first token, it's the whole object
	int currentToken = 1;

	// Scan each keys
	for (int i = 0; i < tokens[0].size / 2 ; i++)
	{
		// Get key token string
		char* key = json + tokens[currentToken].start;

		// Compare with desired name
		if (strcmp(name, key) == 0)
		{
			return json + tokens[currentToken + 1].start;
		}

		// move forward: key + value + nested tokens
		currentToken += 2 + getNestedTokenCounts(currentToken + 1);
	}

	return NULL;
}

char* JsonArray::getString(int index)
{
	if (json == NULL) return NULL;
	if (tokens == NULL) return NULL;
	if (index < 0) return NULL;
	if (index >= tokens[0].size) return NULL;

	// skip first token, it's the whole object
	int currentToken = 1;

	for (int i = 0; i < index; i++)
	{
		// move forward: current + nested tokens
		currentToken += 1 + getNestedTokenCounts(currentToken);
	}

	return json + tokens[currentToken].start;
}
