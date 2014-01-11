/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#include "JsonArray.h"

jsmntok_t* JsonArray::getToken(int index)
{
	if (json == 0 || tokens == 0 || index < 0 || index >= tokens[0].size)
		return 0;

	// skip first token, it's the whole object
	int currentToken = 1;

	for (int i = 0; i < index; i++)
	{
		// move forward: current + nested tokens
		currentToken += 1 + getNestedTokenCounts(currentToken);
	}

	return &tokens[currentToken];
}

JsonArray JsonArray::getArray(int index)
{
	jsmntok_t* token = getToken(index);
	return JsonArray(json, token);
}