#pragma once

#include "ArduinoJson/JsonObjectKeyValue.hpp"

namespace ArduinoJson
{
	class JsonObject;

	class JsonObjectIterator
	{
		friend class JsonObject;

	public:
		explicit JsonObjectIterator(Internals::JsonNode* node)
			: _node(node)
		{
		}

        const char* key() const
        {
            return operator*().key();
        }

        JsonValue value() const
        {
            return operator*().value();
        }

		void operator++()
		{
			_node = _node->next;
		}

		JsonObjectKeyValue operator*() const
		{
			return JsonObjectKeyValue(_node);
		}

		bool operator==(const JsonObjectIterator& other) const
		{
			return _node == other._node;
		}

		bool operator!=(const JsonObjectIterator& other) const
		{
			return _node != other._node;
		}		

	private:
		Internals::JsonNode* _node;
	};
}