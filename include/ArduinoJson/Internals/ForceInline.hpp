// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#ifdef _MSC_VER
#define JSON_FORCE_INLINE __forceinline
#else
#define JSON_FORCE_INLINE __attribute__((always_inline))
#endif
