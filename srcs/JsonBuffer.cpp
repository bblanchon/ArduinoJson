#include "JsonBuffer.h"
//#include "JsonNode.h"
#include "JsonObject.h"


JsonObject JsonBuffer::createObject()
{
    allocateNode();
    return JsonObject(this);
}

void JsonBuffer::createNode()
{
    allocateNode();
}