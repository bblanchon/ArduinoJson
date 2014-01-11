/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#ifndef __ARDUINOJSONPARSER_H
#define __ARDUINOJSONPARSER_H

#include <Arduino.h>
#include "utility/jsmn.h"

class JsonObjectBase
{
public:

	JsonObjectBase()
	{
		json = NULL;
		tokens = NULL;
	}

	bool success() 
	{ 
		return json != NULL && tokens != NULL;
	}

protected:
	
	JsonObjectBase(char* json, jsmntok_t* tokens)
	{
		this->json = json;
		this->tokens = tokens;
	}
		
	int getNestedTokenCounts(int tokenIndex);

	char* json;
	jsmntok_t* tokens;
};

class JsonHashTable : public JsonObjectBase
{
	friend class JsonParserBase;

public:

	JsonHashTable()
	{

	}

	char* getString(char* key);

private:

	JsonHashTable(char* json, jsmntok_t* tokens)
		: JsonObjectBase(json, tokens)
	{

	}
};

class JsonParserBase
{
public:

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
	/*
	char* getValueByIndex(int index);
	char* getValueByKey(char* name);
	int getArraySize();
	*/
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
/*
template <int N>
class JsonArrayParser : public JsonParserBase
{
public:

	JsonArrayParser()
		: JsonParserBase(tokens, N + 1)
	{

	}

	bool parse(char* json)
	{
		return parseAndCheckType(json, JSMN_ARRAY);
	}

	int getCount()
	{
		return getArraySize();
	}

	char* getValue(int index)
	{
		return getValueByIndex(index);
	}

private:

	jsmntok_t tokens[N + 1];
};
*/
#endif

