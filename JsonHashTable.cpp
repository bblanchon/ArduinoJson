/*
* malloc-free JSON parser for Arduino
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonArray.h"
#include "JsonHashTable.h"

#include <string.h> // for strcmp()

JsonHashTable::JsonHashTable(char* json, jsmntok_t* tokens)
: JsonObjectBase(json, tokens)
{
	if (tokens[0].type != JSMN_OBJECT)
		makeInvalid();
}

/*
* Returns the token for the value associated with the specified key
*/
jsmntok_t* JsonHashTable::getToken(char* desiredKey)
{	
	// sanity check
	if (json == 0 || tokens == 0 || desiredKey == 0)
		return 0;

	// skip first token, it's the whole object
	int currentToken = 1;

	// scan each keys
	for (int i = 0; i < tokens[0].size / 2 ; i++)
	{
		// get key token string
		char* key = json + tokens[currentToken].start;

		// Compare with desired name
		if (strcmp(desiredKey, key) == 0)
		{
			return &tokens[currentToken + 1];
		}

		// move forward: key + value + nested tokens
		currentToken += 2 + getNestedTokenCount(currentToken + 1);
	}

	// nothing found, return NULL
	return 0; 
}

JsonArray JsonHashTable::getArray(char* key)
{
	return JsonArray(json, getToken(key));
}

JsonHashTable JsonHashTable::getHashTable(char* key)
{
	return JsonHashTable(json, getToken(key));
}

char* JsonHashTable::getString(char* key)
{
	jsmntok_t* token = getToken(key);
	return token != 0 ? json + token->start : 0;
}