/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#ifndef __JSONOBJECTBASE_H
#define __JSONOBJECTBASE_H

#include "utility/jsmn.h"

class JsonObjectBase
{
public:

	JsonObjectBase()
	{
		makeInvalid();
	}

	bool success() 
	{ 
		return json != 0 && tokens != 0;
	}

protected:
	
	JsonObjectBase(char* json, jsmntok_t* tokens)
	{
		this->json = json;
		this->tokens = tokens;
	}
		
	void makeInvalid()
	{
		json = 0;
		tokens = 0;
	}
	
	static int getNestedTokenCount(jsmntok_t* token);

	bool getBoolFromToken(jsmntok_t* token);
	double getDoubleFromToken(jsmntok_t* token);
	long getLongFromToken(jsmntok_t* token);
	char* getStringFromToken(jsmntok_t* token);

	char* json;
	jsmntok_t* tokens;
};

#endif

