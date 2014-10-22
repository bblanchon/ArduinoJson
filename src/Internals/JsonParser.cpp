#include "ArduinoJson/Internals/JsonParser.hpp"

#include <stdlib.h> // for strtol, strtod
#include <ctype.h>

#include "ArduinoJson/JsonBuffer.hpp"
#include "ArduinoJson/Internals/QuotedString.hpp"

using namespace ArduinoJson::Internals;

bool JsonParser::isArrayStart()
{
    return *_ptr == '[';
}

bool JsonParser::isBoolean()
{
    return *_ptr == 't' || *_ptr == 'f';
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

bool JsonParser::isEnd()
{
    return *_ptr == 0;
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

bool JsonParser::isNull()
{
    return *_ptr == 'n';
}

bool JsonParser::isObjectStart()
{
    return *_ptr == '{';
}

void JsonParser::skipSpaces()
{
    while(isspace(*_ptr)) _ptr++;
}

bool JsonParser::skip(char charToSkip)
{
    skipSpaces();
    if (*_ptr != charToSkip) return false;
    _ptr++;
    skipSpaces();
    return true;
}

JsonNode* JsonParser::parseAnything()
{
    skipSpaces();

    if (isArrayStart())
        return parseArray();

    if (isBoolean())
        return parseBoolean();
        
    if (isDouble())
        return parseDouble();

    if (isLong())    
        return parseLong();

    if (isNull())
        return parseNull();

    if (isObjectStart())
        return parseObject();

    return parseString();
}

JsonNode* JsonParser::parseArray()
{
    JsonNode* node = _buffer->createArrayNode();

    skip('[');

    if (isEnd())
        return 0;

    if (skip(']'))
        return node; // empty array

    for(;;)
    {
        JsonNode* child = parseAnything();

        if (!child)
            return 0; // child parsing failed

        node->addChild(child);

        if (skip(']'))
            return node; // end of the array

        if (!skip(','))
            return 0; // comma is missing
    }
}

JsonNode *JsonParser::parseBoolean()
{
    bool value = *_ptr == 't';

    _ptr += value ? 4 : 5;
    // 4 = strlen("true")
    // 5 = strlen("false");

    return _buffer->createBoolNode(value);
}

JsonNode *JsonParser::parseDouble()
{
    double value = strtod(_ptr, &_ptr);

    int decimals = 0;
    while(_ptr[1-decimals] != '.')
        decimals++;

    return _buffer->createDoubleNode(value, decimals);
}

JsonNode* JsonParser::parseLong()
{
    long value = strtol(_ptr, &_ptr, 10);

    return _buffer->createLongNode(value);
}

JsonNode* JsonParser::parseNull()
{
    _ptr += 4; // strlen("null")

    return _buffer->createStringNode(0);
}

JsonNode* JsonParser::parseObject()
{
    JsonNode* node = _buffer->createObjectNode();

    skip('{');

    if (isEnd())
        return 0; // premature ending

    if (skip('}'))
        return node; // empty object

    for(;;)
    {
        JsonNode* child = parseObjectKeyValue();

        if (!child)
            return 0; // child parsing failed

        node->addChild(child);

        if (skip('}'))
            return node; // end of the object

        if (!skip(','))
            return 0; // comma is missing
    }
}

JsonNode* JsonParser::parseObjectKeyValue()
{
    const char* key = QuotedString::extractFrom(_ptr, &_ptr);

    if (!key)
        return 0; // failed to extract key

    if (!skip(':'))
        return 0; // colon is missing

    JsonNode* value = parseAnything();

    if (!value)
        return 0; // value parsing failed

    return _buffer->createObjectKeyValueNode(key, value);
}

JsonNode* JsonParser::parseString()
{
    const char* s = QuotedString::extractFrom(_ptr, &_ptr);
    return _buffer->createStringNode(s);
}
