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

	JsonNode* parseNode();
    JsonNode* parseArray();
    JsonNode* parseLong();
private:
	JsonBuffer* _buffer;
	char* _ptr;

	inline bool isEnd();
	inline bool isArrayStart();
	inline bool isArrayStop();
	inline bool isLong();
	inline bool isSpace();
	inline void skipOneChar();
	inline void skipSpaces();
};