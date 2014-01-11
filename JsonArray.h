/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#ifndef __JSONARRAY_H
#define __JSONARRAY_H

#include "JsonObjectBase.h"

class JsonArray : public JsonObjectBase
{
	friend class JsonParserBase;
	friend class JsonHashTable;

public:

public:

	JsonArray()
	{

	}

	JsonArray getArray(int index);

	char* getString(int index)
	{
		jsmntok_t* token = getToken(index);
		return token != 0 ? json + token->start : 0;
	}
	
	int getLength()
	{
		return tokens != 0 ? tokens[0].size : 0;
	}

private:

	JsonArray(char* json, jsmntok_t* tokens)
		: JsonObjectBase(json, tokens)
	{

	}

	jsmntok_t* getToken(int index);
};

#endif

