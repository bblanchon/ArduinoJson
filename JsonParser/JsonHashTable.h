/*
* malloc-free JSON parser for Arduino
* Benoit Blanchon 2014 - MIT License
*/

#ifndef __JSONHASHTABLE_H
#define __JSONHASHTABLE_H

#include "JsonObjectBase.h"

class JsonArray;

class JsonHashTable : public JsonObjectBase
{
	template <int N>
	friend class JsonParser;

	friend class JsonArray;

public:

	JsonHashTable() {}

	bool containsKey(const char* key);

	JsonArray getArray(const char* key);
	bool getBool(const char* key);
	double getDouble(const char* key);
	JsonHashTable getHashTable(const char* key);
	long getLong(const char* key);
	char* getString(const char* key);

private:

	JsonHashTable(char* json, jsmntok_t* tokens);
	jsmntok_t* getToken(const char* key);
};

#endif