/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#include "JsonObjectBase.h"

int JsonObjectBase::getNestedTokenCounts(int tokenIndex)
{
	int count = 0;

	for (int i = 0; i < tokens[tokenIndex].size; i++)
	{
		count += 1 + getNestedTokenCounts(tokenIndex + 1 + i);
	}

	return count;
}