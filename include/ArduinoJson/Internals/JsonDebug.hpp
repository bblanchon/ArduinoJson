// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#ifdef JSON_DEBUG
#include <Arduino.h>
#define JSON_DEBUG_PRINT(string) (Serial.print(string))
#define JSON_DEBUG_PRINTLN(string) (Serial.println(string))
#endif

#ifndef JSON_DEBUG
#define JSON_DEBUG_PRINT(string)
#define JSON_DEBUG_PRINTLN(string)
#endif
