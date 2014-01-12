/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#include "JsonObjectBase.h"

#include <stdlib.h> // for strtol

int JsonObjectBase::getNestedTokenCount(jsmntok_t* token)
{
	int count = 0;

	for (int i = 0; i < token->size; i++)
	{
		count += 1 + getNestedTokenCount(token + 1 + i);
	}

	return count;
}

long JsonObjectBase::getLongFromToken(jsmntok_t* token)
{
	if (token->type != JSMN_PRIMITIVE)
		return 0;

	return strtol(json + token->start, 0, 0);
}

char* JsonObjectBase::getStringFromToken(jsmntok_t* token)
{
	if (token->type != JSMN_PRIMITIVE && token->type != JSMN_STRING)
		return 0;

	// add null terminator to the string
	json[token->end] = 0;

	return json + token->start;
}