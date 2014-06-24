/*
* malloc-free JSON parser for Arduino
* Benoit Blanchon 2014 - MIT License
*/

#ifndef __JSONPARSER_H
#define __JSONPARSER_H

#include "JsonHashTable.h"
#include "JsonArray.h"

/*
* The JSON parser.
*
* You need to specifiy the number of token to be allocated for that parser.
* Values from 16 to 32 are recommended.
* The parser size will be MAX_TOKEN*8 bytes.
* Don't forget that the memory size of standard Arduino board is only 2KB
*
* CAUTION: JsonArray and JsonHashTable contain pointers to tokens of the
* JsonParser, so they need the JsonParser to be in memory to work.
* As a result, you must not create JsonArray and JsonHashTable that have a 
* longer life that the JsonParser.
*/
template <int MAX_TOKENS>
class JsonParser
{
public:

	/*
	* Parse the JSON string and return a array.
	*
	* The content of the string may be altered to add '\0' at the
	* end of string tokens
	*/ 
	JsonArray parseArray(char* json)
	{
		return JsonArray(json, parse(json));
	}

	/*
	* Parse the JSON string and return a array.
	*
	* The content of the string may be altered to add '\0' at the
	* end of string tokens
	*/
	JsonHashTable parseHashTable(char* json)
	{
		return JsonHashTable(json, parse(json));
	}

private:

	jsmntok_t* parse(char* json)
	{
		jsmn_parser parser;
		jsmn_init(&parser);

		if (JSMN_SUCCESS != jsmn_parse(&parser, json, tokens, MAX_TOKENS))
			return 0;

		return tokens;
	}

	jsmntok_t tokens[MAX_TOKENS];
};

#endif

