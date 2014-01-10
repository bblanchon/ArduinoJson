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
protected:

	JsonParserBase(jsmntok_t* tokens, int tokenCount)
	{
		this->tokenCount = tokenCount;
		this->tokens = tokens;

		jsmn_init(&parser);
	}
	
	bool parseTokens(char* jsonString);
	char* getValueByKey(char* name);

private:

	char* buffer;
	jsmn_parser parser;
	int tokenCount;
	jsmntok_t* tokens;
};

template <int N>
class ArduinoJsonParser : JsonParserBase
{
public:

	ArduinoJsonParser()
		: JsonParserBase(tokens, N * 2 + 1)
	{

	}

	bool parse(char* json)
	{
		return parseTokens(json);
	}
	
	char* getValue(char* name)
	{
		return getValueByKey(name);
	}

private:
	
	jsmntok_t tokens[N * 2 + 1];
};

#endif

