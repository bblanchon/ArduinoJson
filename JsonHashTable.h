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
	friend class JsonArray;
	friend class JsonParserBase;

public:

	JsonHashTable() {}

	JsonArray getArray(char* key);
	JsonHashTable getHashTable(char* key);
	char* getString(char* key);

private:

	JsonHashTable(char* json, jsmntok_t* tokens)
		: JsonObjectBase(json, tokens)
	{
	}

	jsmntok_t* getToken(char* key);
};

#endif

