/*
* malloc-free JSON parser for Arduino
* Benoit Blanchon 2014 - MIT License
*/

#ifndef __JSONPARSER_H
#define __JSONPARSER_H

#include "JsonHashTable.h"
#include "JsonArray.h"

template <int N>
class JsonParser
{
public:

	JsonArray parseArray(char* json)
	{
		return JsonArray(json, parse(json));
	}

	JsonHashTable parseHashTable(char* json)
	{
		return JsonHashTable(json, parse(json));
	}

private:

	jsmntok_t* parse(char* jsonString)
	{
		jsmn_parser parser;
		jsmn_init(&parser);

		if (JSMN_SUCCESS != jsmn_parse(&parser, jsonString, tokens, N))
			return 0;

		return tokens;
	}

	jsmntok_t tokens[N];
};

#endif

