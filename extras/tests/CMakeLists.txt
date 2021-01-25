# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2021
# MIT License

add_subdirectory(catch)

link_libraries(ArduinoJson catch)

include_directories(Helpers)
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
