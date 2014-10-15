#include "JsonNode.h"

#pragma once

class JsonNode;
class JsonBuffer;

class JsonParser
{
public:
	JsonParser(JsonBuffer* buffer, char* json)
	 	: _buffer(buffer), _ptr(json)
	{

	}

	JsonNode* parseAnything();

private:
	JsonBuffer* _buffer;
	char* _ptr;

	inline bool isEnd();
	inline bool isArrayStart();
	inline bool isArrayStop();
	inline bool isLong();
    inline bool isDouble();
	inline bool isSpace();
    inline bool isComma();
    inline bool isBoolean();

	inline void skipOneChar();
	inline void skipSpaces();

    inline JsonNode* parseArray();
    inline JsonNode* parseLong();
    inline JsonNode* parseBoolean();

    JsonNode *parseDouble();
};