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

		const char* key() const
		{
			return _node->getAsObjectKey();
		}

		JsonValue value()
		{
			return JsonValue(_node->getAsObjectValue());
		}

		void operator++()
		{
			_node = _node->next;
		}

		bool operator==(const JsonObjectKeyValue& other) const
		{
			return _node == other._node;
		}

		bool operator!=(const JsonObjectKeyValue& other) const
		{
			return _node != other._node;
		}		

	private:
		Internals::JsonNode* _node;
	};
}