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

bool JsonParser::isComma()
{
    return *_ptr == ',';
}

void JsonParser::skipOneChar()
{
    _ptr++;
}

void JsonParser::skipSpaces()
{
    while(isSpace()) skipOneChar();
}

JsonNode* JsonParser::parseAnything()
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
    JsonNode* node = _buffer->createNode();
    node->setAsArray(_buffer);

    skipOneChar(); // skip the '['
    skipSpaces();

    if (isEnd())
        return 0;

    if (isArrayStop())
        return node;

    for(;;)
    {
        node->addChild(parseAnything());

        skipSpaces();

        if (isArrayStop())
            return node;

        if (!isComma())
            return 0;

        skipOneChar(); // skip the ','
    }

    return node;
}

JsonNode* JsonParser::parseLong()
{
    long value = strtol(_ptr, &_ptr, 10);

    JsonNode* node = _buffer->createNode();
    node->setAsLong(value);
    return node;
}