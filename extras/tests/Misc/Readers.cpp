// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("StdStreamReader") {
  SECTION("read()") {
    std::istringstream src("\x01\xFF");
    StdStreamReader reader(src);

    REQUIRE(reader.read() == 0x01);
    REQUIRE(reader.read() == 0xFF);
    REQUIRE(reader.read() == -1);
  }

  SECTION("readBytes() all at once") {
    std::istringstream src("ABC");
    StdStreamReader reader(src);

    char buffer[8] = "abcd";
    REQUIRE(reader.readBytes(buffer, 4) == 3);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'd');
  }

  SECTION("readBytes() in two parts") {
    std::istringstream src("ABCDEF");
    StdStreamReader reader(src);

    char buffer[12] = "abcdefg";
    REQUIRE(reader.readBytes(buffer, 4) == 4);
    REQUIRE(reader.readBytes(buffer + 4, 4) == 2);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'D');
    REQUIRE(buffer[4] == 'E');
    REQUIRE(buffer[5] == 'F');
    REQUIRE(buffer[6] == 'g');
  }
}

TEST_CASE("SafeCharPointerReader") {
  SECTION("read") {
    SafeCharPointerReader reader("\x01\xFF", 2);
    REQUIRE(reader.read() == 0x01);
    REQUIRE(reader.read() == 0xFF);
    REQUIRE(reader.read() == -1);
    REQUIRE(reader.read() == -1);
  }

  SECTION("readBytes() all at once") {
    SafeCharPointerReader reader("ABCD", 3);

    char buffer[8] = "abcd";
    REQUIRE(reader.readBytes(buffer, 4) == 3);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'd');
  }

  SECTION("readBytes() in two parts") {
    SafeCharPointerReader reader("ABCDEF", 6);

    char buffer[8] = "abcdefg";
    REQUIRE(reader.readBytes(buffer, 4) == 4);
    REQUIRE(reader.readBytes(buffer + 4, 4) == 2);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'D');
    REQUIRE(buffer[4] == 'E');
    REQUIRE(buffer[5] == 'F');
    REQUIRE(buffer[6] == 'g');
  }
}

TEST_CASE("UnsafeCharPointerReader") {
  SECTION("read()") {
    UnsafeCharPointerReader reader("\x01\xFF\x00\x12");
    REQUIRE(reader.read() == 0x01);
    REQUIRE(reader.read() == 0xFF);
    REQUIRE(reader.read() == 0);
    REQUIRE(reader.read() == 0x12);
  }

  SECTION("readBytes() all at once") {
    UnsafeCharPointerReader reader("ABCD");

    char buffer[8] = "abcd";
    REQUIRE(reader.readBytes(buffer, 3) == 3);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'd');
  }

  SECTION("readBytes() in two parts") {
    UnsafeCharPointerReader reader("ABCDEF");

    char buffer[8] = "abcdefg";
    REQUIRE(reader.readBytes(buffer, 4) == 4);
    REQUIRE(reader.readBytes(buffer + 4, 2) == 2);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'D');
    REQUIRE(buffer[4] == 'E');
    REQUIRE(buffer[5] == 'F');
    REQUIRE(buffer[6] == 'g');
  }
}

TEST_CASE("IteratorReader") {
  SECTION("read()") {
    std::string src("\x01\xFF");
    IteratorReader<std::string::const_iterator> reader(src.begin(), src.end());

    REQUIRE(reader.read() == 0x01);
    REQUIRE(reader.read() == 0xFF);
    REQUIRE(reader.read() == -1);
  }

  SECTION("readBytes() all at once") {
    std::string src("ABC");
    IteratorReader<std::string::const_iterator> reader(src.begin(), src.end());

    char buffer[8] = "abcd";
    REQUIRE(reader.readBytes(buffer, 4) == 3);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'd');
  }

  SECTION("readBytes() in two parts") {
    std::string src("ABCDEF");
    IteratorReader<std::string::const_iterator> reader(src.begin(), src.end());

    char buffer[12] = "abcdefg";
    REQUIRE(reader.readBytes(buffer, 4) == 4);
    REQUIRE(reader.readBytes(buffer + 4, 4) == 2);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'D');
    REQUIRE(buffer[4] == 'E');
    REQUIRE(buffer[5] == 'F');
    REQUIRE(buffer[6] == 'g');
  }
}
