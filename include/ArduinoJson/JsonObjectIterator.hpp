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
			: _objectKeyValue(node)
		{
		}

		void operator++()
		{
			++_objectKeyValue;
		}

		JsonObjectKeyValue operator*() const
		{
			return _objectKeyValue;
		}

        JsonObjectKeyValue* operator->()
		{
			return &_objectKeyValue;
		}

		bool operator==(const JsonObjectIterator& other) const
		{
			return _objectKeyValue == other._objectKeyValue;
		}

		bool operator!=(const JsonObjectIterator& other) const
		{
			return _objectKeyValue != other._objectKeyValue;
		}

	private:
		JsonObjectKeyValue _objectKeyValue;
	};
}