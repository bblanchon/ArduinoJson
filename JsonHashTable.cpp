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
	if (tokens == 0 || tokens[0].type != JSMN_OBJECT)
		makeInvalid();
}

/*
* Returns the token for the value associated with the specified key
*/
jsmntok_t* JsonHashTable::getToken(const char* desiredKey)
{	
	// sanity check
	if (json == 0 || tokens == 0 || desiredKey == 0)
		return 0;

	// skip first token, it's the whole object
	jsmntok_t* currentToken = tokens + 1;

	// scan each keys
	for (int i = 0; i < tokens[0].size / 2 ; i++)
	{
		// get key token string
		char* key = getStringFromToken(currentToken);

		// compare with desired name
		if (strcmp(desiredKey, key) == 0)
		{
			// return the value token that follows the key token
			return currentToken + 1;
		}

		// move forward: key + value + nested tokens
		currentToken += 2 + getNestedTokenCount(currentToken + 1);
	}

	// nothing found, return NULL
	return 0; 
}

bool JsonHashTable::containsKey(const char* key)
{
	return getToken(key) != 0;
}

JsonArray JsonHashTable::getArray(const char* key)
{
	return JsonArray(json, getToken(key));
}

bool JsonHashTable::getBool(const char* key)
{
	return getBoolFromToken(getToken(key));
}

double JsonHashTable::getDouble(const char* key)
{
	return getDoubleFromToken(getToken(key));
}

JsonHashTable JsonHashTable::getHashTable(const char* key)
{
	return JsonHashTable(json, getToken(key));
}

long JsonHashTable::getLong(const char* key)
{
	return getLongFromToken(getToken(key));
}

char* JsonHashTable::getString(const char* key)
{
	return getStringFromToken(getToken(key));
}