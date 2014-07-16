/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include <string.h> // for strcmp()
#include "JsonHashTable.h"
#include "JsonArray.h"
#include "JsonValue.h"

using namespace ArduinoJson::Parser;

JsonHashTable::JsonHashTable(char* json, jsmntok_t* tokens)
: JsonObjectBase(json, tokens)
{
	if (tokens == 0 || tokens[0].type != JSMN_OBJECT)
		makeInvalid();
}

/*
* Returns the token for the value associated with the specified key
*/
JsonValue JsonHashTable::operator [](const char* desiredKey)
{	
	// sanity check
	if (json == 0 || tokens == 0 || desiredKey == 0)
        return JsonValue();

	// skip first token, it's the whole object
	jsmntok_t* currentToken = tokens + 1;

	// scan each keys
	for (int i = 0; i < tokens[0].size / 2 ; i++)
	{
		// get key token string
		char* key = JsonValue(json, currentToken);

		// compare with desired name
		if (strcmp(desiredKey, key) == 0)
		{
			// return the value token that follows the key token
			return JsonValue(json, currentToken + 1);
		}

		// move forward: key + value + nested tokens
		currentToken += 2 + getNestedTokenCount(currentToken + 1);
	}

	// nothing found, return NULL
    return JsonValue();
}

JsonArray JsonHashTable::getArray(const char* key) DEPRECATED
{
    return (JsonArray) (*this)[key];
}