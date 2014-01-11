/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#ifndef __JSONARRAY_H
#define __JSONARRAY_H

#include "JsonObjectBase.h"

class JsonHashTable;

class JsonArray : public JsonObjectBase
{
	friend class JsonParserBase;
	friend class JsonHashTable;

public:

	JsonArray()	{}

	int getLength()
	{
		return tokens != 0 ? tokens[0].size : 0;
	}

	JsonArray getArray(int index);	

	JsonHashTable getHashTable(int index);

	char* getString(int index);

private:

	JsonArray(char* json, jsmntok_t* tokens)
		: JsonObjectBase(json, tokens)
	{
	}

	jsmntok_t* getToken(int index);
};

#endif

