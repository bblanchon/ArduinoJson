#pragma once

#include "ArduinoJson/JsonValue.hpp"

namespace ArduinoJson
{
	class JsonObjectKeyValue
	{
	public:
		explicit JsonObjectKeyValue(Internals::JsonNode* node)
			: _node(node)
		{			
		}

		const char* key()
		{
			return _node->getAsObjectKey();
		}

		JsonValue value()
		{
			return JsonValue(_node->getAsObjectValue());
		}

	private:
		Internals::JsonNode* _node;
	};
}