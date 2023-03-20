// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Document/BasicJsonDocument.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A JsonDocument with a memory pool in the heap.
// https://arduinojson.org/v6/api/dynamicjsondocument/
typedef BasicJsonDocument DynamicJsonDocument;

ARDUINOJSON_END_PUBLIC_NAMESPACE
