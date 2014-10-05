#include "JsonValue.h"

#include "JsonArray.h"
#include "JsonObject.h"
#include "Internals/JsonNode.h"

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
    set(value, 2);
}

void JsonValue::set(double value, int decimals)
{
    if (!_node) return;

    _node->type = (JsonNodeType) (JSON_DOUBLE_0_DECIMALS + decimals);
    _node->content.asDouble = value;
}

void JsonValue::operator=(int value)
{
    if (!_node) return;

    _node->type = JSON_INTEGER;
    _node->content.asInteger = value;
}

void JsonValue::operator=(const JsonContainer& object)
{
    setAsProxyTo(object._node);
}

void JsonValue::operator=(JsonValue const& value)
{
    if (!_node)
    {
        _node = value._node;
        return;
    }
    
    JsonNodeType nodeType = value._node ? value._node->type : JSON_UNDEFINED;    

    switch (nodeType)
    {
    case JSON_UNDEFINED:
        _node->type = JSON_UNDEFINED;
        break;

    case JSON_INTEGER:
        _node->type = JSON_INTEGER;
        _node->content.asInteger = value._node->content.asInteger;
        break;

    case JSON_DOUBLE_0_DECIMALS:

    case JSON_OBJECT:
    case JSON_ARRAY:
    case JSON_PROXY:
        setAsProxyTo(value._node);
        break;

    default:
        *_node = *value._node;
        break;
    }
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

JsonValue::operator JsonArray() const
{
    return JsonArray(getActualNode());
}

JsonValue::operator JsonObject() const
{
    return JsonObject(getActualNode());
}

void JsonValue::setAsProxyTo(JsonNode* target)
{
    if (_node)
    {
        _node->type = JSON_PROXY;
        _node->content.asProxy.target = target;
    }

    _node = target;
}

JsonNode* JsonValue::getActualNode() const
{
    JsonNode* target = _node;

    while (target && target->type == JSON_PROXY)
        target = target->content.asProxy.target;

    return target;
}