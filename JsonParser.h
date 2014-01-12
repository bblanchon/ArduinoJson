/*
* malloc-free JSON parser for Arduino
* Benoit Blanchon 2014 - MIT License
*/

#ifndef __JSONPARSER_H
#define __JSONPARSER_H

#include "JsonHashTable.h"
#include "JsonArray.h"

template <int SIZE> // SIZE of the parser in bytes  (128, 256 or more are recommended)
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

	jsmntok_t* parse(char* json)
	{
		jsmn_parser parser;
		jsmn_init(&parser);

		jsmntok_t* tokens = (jsmntok_t*) buffer;
		int maxTokenCount = SIZE / sizeof(jsmntok_t);

		if (JSMN_SUCCESS != jsmn_parse(&parser, json, tokens, maxTokenCount))
			return 0;

		return tokens;
	}

	char buffer[SIZE];
};

#endif

