#include "JsonObject.h"
#include "JsonNode.h"
#include "JsonValue.h"

//void JsonValue::operator=(JsonObject const& object)
//{
//    _node = object._node;
//}

void JsonValue::operator=(int value)
{
    if (!_node) return;

    _node->type = JSON_INTEGER;
    _node->content.asInteger = value;
}

//JsonValue::operator JsonObject()
//{
//    return JsonObject(_buffer, _node);
//}

JsonValue::operator int()
{
    if (!_node || _node->type != JSON_INTEGER) return 0;

    return _node->content.asInteger;
}