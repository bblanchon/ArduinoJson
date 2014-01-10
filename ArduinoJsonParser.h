/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#ifndef __ARDUINOJSONPARSER_H
#define __ARDUINOJSONPARSER_H

#include <Arduino.h>
#include "utility/jsmn.h"

class JsonParserBase
{
public:

protected:

	JsonParserBase(jsmntok_t* tokens, int maxTokenCount)
	{
		this->maxTokenCount = maxTokenCount;
		this->tokens = tokens;

		jsmn_init(&parser);
	}	
	
	int getTokenCount()
	{
		return parser.toknext - 1;
	}

	bool parseAndCheckType(char* json, jsmntype_t type);
	char* getValueByIndex(int index);
	char* getValueByKey(char* name);

private:

	char* buffer;
	jsmn_parser parser;
	int maxTokenCount;
	jsmntok_t* tokens;
};

template <int N>
class JsonObjectParser : public JsonParserBase
{
public:

	JsonObjectParser()
		: JsonParserBase(tokens, N * 2 + 1)
	{

	}

	bool parse(char* json)
	{
		return parseAndCheckType(json, JSMN_OBJECT);
	}

	char* getValue(char* name)
	{
		return getValueByKey(name);
	}

private:
	
	jsmntok_t tokens[N * 2 + 1];
};

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
		return getTokenCount();
	}

	char* getValue(int index)
	{
		return getValueByIndex(index);
	}

private:

	jsmntok_t tokens[N + 1];
};

#endif

