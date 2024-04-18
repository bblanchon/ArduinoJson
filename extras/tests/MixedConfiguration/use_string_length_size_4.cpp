#define ARDUINOJSON_STRING_LENGTH_SIZE 4
#include <ArduinoJson.h>

#include <array>
#include <catch.hpp>

TEST_CASE("ARDUINOJSON_STRING_LENGTH_SIZE == 4") {
  SECTION("deserializeMsgPack() bin 32") {
    JsonDocument doc;

    static const auto array = std::array<char, 0x10000>({5});
    auto input = std::string("\xc6\x00\x01\x00\x00", 5) +
                 std::string(array.data(), array.size());

    DeserializationError error = deserializeMsgPack(doc, input);

    REQUIRE(error == DeserializationError::Ok);
    REQUIRE(doc.is<MsgPackBinary>());
    auto binary = doc.as<MsgPackBinary>();
    REQUIRE(binary.size() == 0x10000);
    REQUIRE(binary.data() != nullptr);
    REQUIRE(reinterpret_cast<const char*>(binary.data())[0] == 5);
  }

  SECTION("serializeMsgPack() bin 32") {
    static const auto bin32 = std::array<char, 0x10000>({1, 2, 3, 4});
    std::string bin32String(bin32.data(), bin32.size());

    printf("Max s length %x\n", detail::StringNode::maxLength);
    JsonDocument doc;
    JsonVariant variant = doc.to<JsonVariant>();
    variant.set(MsgPackBinary(bin32.data(), bin32.size()));
    std::string expected = std::string("\xC6\x00\x01\x00\x00", 5) + bin32String;
    std::string actual;
    size_t len = serializeMsgPack(variant, actual);
    CAPTURE(variant);
    CAPTURE(actual);
    CAPTURE(expected);
    REQUIRE(len == expected.size());
    REQUIRE(actual == expected);
  }
}