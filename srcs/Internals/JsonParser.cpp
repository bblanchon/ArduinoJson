#include "JsonParser.h"
#include "../JsonBuffer.h"
#include <stdlib.h> // for strtol, strtod
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

    // same position => 0 digits => not a number
    if (ptr == _ptr) 
        return false;

    // stopped on a decimal separator => not a long but a double
    return *ptr != '.';
}

bool JsonParser::isDouble()
{
    char* ptr = _ptr;

    // skip all digits
    while (isdigit(*ptr))
        ptr++;

    // same position => 0 digits => not a number
    if (ptr == _ptr)
        return false;

    // stopped on a decimal separator => ok it's a double
    return *ptr == '.';
}

bool JsonParser::isSpace()
{
    return *_ptr == ' ' || *_ptr == '\t' || *_ptr == '\n' || *_ptr == '\r';
}

bool JsonParser::isComma()
{
    return *_ptr == ',';
}

bool JsonParser::isBoolean()
{
    return *_ptr == 't' || *_ptr == 'f';
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

    if (isDouble())
        return parseDouble();

    if (isBoolean())
        return parseBoolean();

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
}

JsonNode* JsonParser::parseLong()
{
    long value = strtol(_ptr, &_ptr, 10);

    return _buffer->createLongNode(value);
}

JsonNode *JsonParser::parseDouble()
{
    double value = strtod(_ptr, &_ptr);

    int decimals = 0;
    while(_ptr[1-decimals] != '.')
        decimals++;

    return _buffer->createDoubleNode(value, decimals);
}

JsonNode *JsonParser::parseBoolean()
{
    bool value = *_ptr == 't';

    _ptr += value ? 4 : 5;
    // 4 = strlen("true")
    // 5 = strlen("false");

    return _buffer->createBoolNode(value);
}
