#include "JsonParser.h"
#include "../JsonBuffer.h"
#include <stdlib.h>
#include <ctype.h>

bool JsonParser::isEnd()
{
    return *_ptr == 0;
}

bool JsonParser::isArrayStart()
{
    return *_ptr == '[';
}

bool JsonParser::isArrayStop()
{
    return *_ptr == ']';
}

bool JsonParser::isLong()
{
    char* ptr = _ptr;

    // skip all digits
    while (isdigit(*ptr))
        ptr++;

    // same position => 0 digits => not a long
    if (ptr == _ptr) 
        return false;

    // stopped on a decimal separator => not a long but a double
    if (*ptr == '.')
        return false;

    return true;
}

bool JsonParser::isSpace()
{
    return *_ptr == ' ' || *_ptr == '\t' || *_ptr == '\n' || *_ptr == '\r';
}

void JsonParser::skipOneChar()
{
    _ptr++;
}

void JsonParser::skipSpaces()
{
    while(isSpace()) skipOneChar();
}

JsonNode* JsonParser::parseNode()
{
    skipSpaces();

    if (isArrayStart())
        return parseArray();
        
    if (isLong())    
        return parseLong();    

    return 0;
}

JsonNode* JsonParser::parseArray()
{
    skipOneChar(); // skip the '['
    skipSpaces();

    JsonNode* node = _buffer->createNode();
    node->setAsArray(_buffer);

    if (isEnd())
        return 0;

    if (isArrayStop())
        return node;

    node->addChild(parseNode());

    return node;
}

JsonNode* JsonParser::parseLong()
{
    long value = strtol(_ptr, &_ptr, 10);

    JsonNode* node = _buffer->createNode();
    node->setAsLong(value);
    return node;
}