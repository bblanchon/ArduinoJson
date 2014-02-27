/*
* malloc-free JSON parser for Arduino
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonArray.h"
#include "JsonHashTable.h"

JsonArray::JsonArray(char* json, jsmntok_t* tokens)
: JsonObjectBase(json, tokens)
{
	if (tokens == 0 || tokens[0].type != JSMN_ARRAY)
		makeInvalid();
}


/*
* Returns the token for the value at the specified index
*/
jsmntok_t* JsonArray::getToken(int index)
{
	// sanity check
	if (json == 0 || tokens == 0 || index < 0 || index >= tokens[0].size)
		return 0;

	// skip first token, it's the whole object
	jsmntok_t* currentToken = tokens + 1;

	// skip all tokens before the specified index
	for (int i = 0; i < index; i++)
	{
		// move forward: current + nested tokens
		currentToken += 1 + getNestedTokenCount(currentToken);
	}

	return currentToken;
}

JsonArray JsonArray::getArray(int index)
{
	return JsonArray(json, getToken(index));
}

bool JsonArray::getBool(int index)
{
	return getBoolFromToken(getToken(index));
}

double JsonArray::getDouble(int index)
{
	return getDoubleFromToken(getToken(index));
}

JsonHashTable JsonArray::getHashTable(int index)
{
	return JsonHashTable(json, getToken(index));
}

long JsonArray::getLong(int index)
{
	return getLongFromToken(getToken(index));
}

char* JsonArray::getString(int index)
{
	return getStringFromToken(getToken(index));
}