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

void JsonValue::operator=(const JsonObject& object)
{
    if (_node) 
    {
        _node->type = JSON_PROXY;
        _node->content.asProxy.target = object._node;
    }
    
    _node = object._node;
}

JsonValue::operator bool() const
{
    const JsonNode* node = getActualNode();

    if (!node || node->type != JSON_BOOLEAN) return 0;

    return node->content.asBoolean;
}

JsonValue::operator char const*() const
{
    const JsonNode* node = getActualNode();

    if (!node || node->type != JSON_STRING) return 0;

    return node->content.asString;
}

JsonValue::operator double() const
{
    const JsonNode* node = getActualNode();

    if (!node || node->type < JSON_DOUBLE_0_DECIMALS) return 0;

    return node->content.asDouble;
}

JsonValue::operator int() const
{
    const JsonNode* node = getActualNode();

    if (!node || node->type != JSON_INTEGER) return 0;

    return node->content.asInteger;
}

JsonValue::operator JsonObject() const
{
    return JsonObject(getActualNode());
}

JsonNode* JsonValue::getActualNode() const
{
    JsonNode* target = _node;

    while (target && target->type == JSON_PROXY)
        target = target->content.asProxy.target;

    return target;
}