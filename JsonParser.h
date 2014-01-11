/*
* malloc-free JSON parser for Arduino
* Benoit Blanchon 2014 - MIT License
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
		return JsonArray(json, parse(json));
	}

	JsonHashTable parseHashTable(char* json)
	{
		return JsonHashTable(json, parse(json));
	}
	
protected:

	JsonParserBase(jsmntok_t* tokens, int maxTokenCount);	
	
private:

	jsmntok_t* parse(char* json);

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

