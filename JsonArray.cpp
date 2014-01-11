/*
* malloc-free JSON parser for Arduino
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonArray.h"
#include "JsonHashTable.h"

JsonArray::JsonArray(char* json, jsmntok_t* tokens)
: JsonObjectBase(json, tokens)
{
	if (tokens[0].type != JSMN_ARRAY)
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
	int currentToken = 1;

	// skip all tokens before the specified index
	for (int i = 0; i < index; i++)
	{
		// move forward: current + nested tokens
		currentToken += 1 + getNestedTokenCount(currentToken);
	}

	return &tokens[currentToken];
}

JsonArray JsonArray::getArray(int index)
{
	return JsonArray(json, getToken(index));
}

JsonHashTable JsonArray::getHashTable(int index)
{
	return JsonHashTable(json, getToken(index));
}

char* JsonArray::getString(int index)
{
	jsmntok_t* token = getToken(index);
	return token != 0 ? json + token->start : 0;
}