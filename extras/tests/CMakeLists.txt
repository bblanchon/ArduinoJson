# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2022, Benoit BLANCHON
# MIT License

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(catch)

link_libraries(ArduinoJson catch)

include_directories(Helpers)
add_subdirectory(Cpp11)
add_subdirectory(Cpp17)
add_subdirectory(Cpp20)
add_subdirectory(FailingBuilds)
add_subdirectory(IntegrationTests)
add_subdirectory(JsonArray)
add_subdirectory(JsonDeserializer)
add_subdirectory(JsonDocument)
add_subdirectory(JsonObject)
add_subdirectory(JsonSerializer)
add_subdirectory(JsonVariant)
add_subdirectory(MemoryPool)
add_subdirectory(Misc)
add_subdirectory(MixedConfiguration)
add_subdirectory(MsgPackDeserializer)
add_subdirectory(MsgPackSerializer)
add_subdirectory(Numbers)
add_subdirectory(TextFormatter)
