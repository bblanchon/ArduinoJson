/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#include "JsonArray.h"
#include "JsonHashTable.h"

#include <string.h> // for strcmp()

jsmntok_t* JsonHashTable::getToken(char* name)
{	
	// sanity check
	if (json == 0 || tokens == 0 || name == 0)
		return 0;

	// skip first token, it's the whole object
	int currentToken = 1;

	// scan each keys
	for (int i = 0; i < tokens[0].size / 2 ; i++)
	{
		// Get key token string
		char* key = json + tokens[currentToken].start;

		// Compare with desired name
		if (strcmp(name, key) == 0)
		{
			return &tokens[currentToken + 1];
		}

		// move forward: key + value + nested tokens
		currentToken += 2 + getNestedTokenCounts(currentToken + 1);
	}

	// nothing found, return NULL
	return 0; 
}

JsonArray JsonHashTable::getArray(char* key)
{
	jsmntok_t* token = getToken(key);
	return JsonArray(json, token);
}