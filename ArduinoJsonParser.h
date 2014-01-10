/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#ifndef __ARDUINOJSONPARSER_H
#define __ARDUINOJSONPARSER_H

#include <Arduino.h>
#include "utility/jsmn.h"

template <int N>
class ArduinoJsonParser
{
public:

	ArduinoJsonParser()
	{
		jsmn_init(&parser);
	}

	boolean parse(char* jsonString)
	{
		buffer = jsonString;

		int tokenCount = sizeof(tokens) / sizeof(tokens[0]);

		if (JSMN_SUCCESS != jsmn_parse(&parser, jsonString, tokens, tokenCount))
			return false;

		// Add null termination to each token
		for (int i = 0; i < tokenCount; i++)
		{
			buffer[tokens[i].end] = 0;
		}

		return true;
	}

	char* getValue(char* name)
	{
		// Scan each keys, every two other token
		// (skip index 0, because it's the whole json object)
		for (int i = 1; i < 2 * N; i += 2)
		{
			// Early break if we reach the last token
			if (i >= parser.toknext) break;

			// Get key token string
			char* key = buffer + tokens[i].start;

			// Compare with desired name, ignoring case
			if (strcasecmp(name, key) == 0)
			{
				return buffer + tokens[i + 1].start;
			}
		}
	}

private:

	char* buffer;
	jsmn_parser parser;
	jsmntok_t tokens[N * 2 + 1];
};

#endif

