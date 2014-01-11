/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#ifndef __JSONPARSER_H
#define __JSONPARSER_H

#include "JsonHashTable.h"
#include "JsonArray.h"

class JsonParserBase
{
public:

	JsonArray parseArray(char* json)
	{
		if (!parse(json) || tokens[0].type != JSMN_ARRAY)
			return JsonArray();

		return JsonArray(json, tokens);
	}

	JsonHashTable parseHashTable(char* json)
	{
		if (!parse(json) || tokens[0].type != JSMN_OBJECT)
			return JsonHashTable();

		return JsonHashTable(json, tokens);
	}
	
protected:

	JsonParserBase(jsmntok_t* tokens, int maxTokenCount)
	{
		this->maxTokenCount = maxTokenCount;
		this->tokens = tokens;

		jsmn_init(&parser);
	}	
	
	bool parse(char* json);

private:

	char* buffer;
	jsmn_parser parser;
	int maxTokenCount;
	jsmntok_t* tokens;
};

template <int N>
class JsonParser : public JsonParserBase
{
public:

	JsonParser()
		: JsonParserBase(tokens, N)
	{

	}

private:
	
	jsmntok_t tokens[N];
};

#endif

