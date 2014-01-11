/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#ifndef __JSONHASHTABLE_H
#define __JSONHASHTABLE_H

#include "JsonObjectBase.h"

class JsonArray;

class JsonHashTable : public JsonObjectBase
{
	friend class JsonParserBase;

public:

	JsonHashTable()
	{

	}

	char* getString(char* key)
	{
		jsmntok_t* token = getToken(key);
		return token != 0 ? json + token->start : 0;
	}

	JsonArray getArray(char* key);

private:

	JsonHashTable(char* json, jsmntok_t* tokens)
		: JsonObjectBase(json, tokens)
	{

	}

	jsmntok_t* getToken(char* key);
};

#endif

