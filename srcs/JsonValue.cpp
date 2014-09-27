#include "JsonObject.h"
#include "JsonNode.h"
#include "JsonValue.h"

void JsonValue::operator=(bool value)
{
    if (!_node) return;

    _node->type = JSON_BOOLEAN;
    _node->content.asBoolean = value;
}

void JsonValue::operator=(char const* value)
{
    if (!_node) return;

    _node->type = JSON_STRING;
    _node->content.asString = value;
}

void JsonValue::operator=(double value)
{
    if (!_node) return;

    _node->type = JSON_DOUBLE_2_DECIMALS;
    _node->content.asDouble = value;
}

void JsonValue::operator=(int value)
{
    if (!_node) return;

    _node->type = JSON_INTEGER;
    _node->content.asInteger = value;
}

JsonValue::operator bool()
{
    if (!_node || _node->type != JSON_BOOLEAN) return 0;

    return _node->content.asBoolean;
}

JsonValue::operator char const*()
{
    if (!_node || _node->type != JSON_STRING) return 0;

    return _node->content.asString;
}

JsonValue::operator double()
{
    if (!_node || _node->type < JSON_DOUBLE_0_DECIMALS) return 0;

    return _node->content.asDouble;
}

JsonValue::operator int()
{
    if (!_node || _node->type != JSON_INTEGER) return 0;

    return _node->content.asInteger;
}