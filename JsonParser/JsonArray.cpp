/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonArray.h"
#include "JsonHashTable.h"

using namespace ArduinoJson::Parser;

JsonArray::JsonArray(char* json, jsmntok_t* tokens)
: JsonObjectBase(json, tokens)
{
	if (tokens == 0 || tokens[0].type != JSMN_ARRAY)
		makeInvalid();
}


/*
* Returns the token for the value at the specified index
*/
JsonValue JsonArray::operator[](int index)
{
	// sanity check
	if (json == 0 || tokens == 0 || index < 0 || index >= tokens[0].size)
        return JsonValue();

	// skip first token, it's the whole object
	jsmntok_t* currentToken = tokens + 1;

	// skip all tokens before the specified index
	for (int i = 0; i < index; i++)
	{
		// move forward: current + nested tokens
		currentToken += 1 + getNestedTokenCount(currentToken);
	}

	return JsonValue(json, currentToken);
}


JsonHashTable JsonArray::getHashTable(int index) DEPRECATED
{
    return (JsonHashTable) (*this)[index];
}
