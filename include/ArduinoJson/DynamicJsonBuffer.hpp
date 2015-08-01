// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/BlockJsonBuffer.hpp"

namespace ArduinoJson {
// Implements a JsonBuffer with dynamic memory allocation.
// You are strongly encouraged to consider using StaticJsonBuffer which is much
// more suitable for embedded systems.
typedef Internals::BlockJsonBuffer<Internals::DefaultAllocator>
    DynamicJsonBuffer;
}
