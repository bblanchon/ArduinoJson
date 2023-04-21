// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <Arduino.h>
#include <ArduinoJson.hpp>
#include <catch.hpp>

#include <sstream>

using namespace ArduinoJson::detail;

TEST_CASE("Reader<std::istringstream>") {
  SECTION("read()") {
    std::istringstream src("\x01\xFF");
    Reader<std::istringstream> reader(src);

    REQUIRE(reader.read() == 0x01);
    REQUIRE(reader.read() == 0xFF);
    REQUIRE(reader.read() == -1);
  }

  SECTION("readBytes() all at once") {
    std::istringstream src("ABC");
    Reader<std::istringstream> reader(src);

    char buffer[8] = "abcd";
    REQUIRE(reader.readBytes(buffer, 4) == 3);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'd');
  }

  SECTION("readBytes() in two parts") {
    std::istringstream src("ABCDEF");
    Reader<std::istringstream> reader(src);

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

TEST_CASE("BoundedReader<const char*>") {
  SECTION("read") {
    BoundedReader<const char*> reader("\x01\xFF", 2);
    REQUIRE(reader.read() == 0x01);
    REQUIRE(reader.read() == 0xFF);
    REQUIRE(reader.read() == -1);
    REQUIRE(reader.read() == -1);
  }

  SECTION("readBytes() all at once") {
    BoundedReader<const char*> reader("ABCD", 3);

    char buffer[8] = "abcd";
    REQUIRE(reader.readBytes(buffer, 4) == 3);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'd');
  }

  SECTION("readBytes() in two parts") {
    BoundedReader<const char*> reader("ABCDEF", 6);

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

TEST_CASE("Reader<const char*>") {
  SECTION("read()") {
    Reader<const char*> reader("\x01\xFF\x00\x12");
    REQUIRE(reader.read() == 0x01);
    REQUIRE(reader.read() == 0xFF);
    REQUIRE(reader.read() == 0);
    REQUIRE(reader.read() == 0x12);
  }

  SECTION("readBytes() all at once") {
    Reader<const char*> reader("ABCD");

    char buffer[8] = "abcd";
    REQUIRE(reader.readBytes(buffer, 3) == 3);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'd');
  }

  SECTION("readBytes() in two parts") {
    Reader<const char*> reader("ABCDEF");

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

class StreamStub : public Stream {
 public:
  StreamStub(const char* s) : stream_(s) {}

  int read() {
    return stream_.get();
  }

  size_t readBytes(char* buffer, size_t length) {
    stream_.read(buffer, static_cast<std::streamsize>(length));
    return static_cast<size_t>(stream_.gcount());
  }

 private:
  std::istringstream stream_;
};

TEST_CASE("Reader<Stream>") {
  SECTION("read()") {
    StreamStub src("\x01\xFF");
    Reader<StreamStub> reader(src);

    REQUIRE(reader.read() == 0x01);
    REQUIRE(reader.read() == 0xFF);
    REQUIRE(reader.read() == -1);
  }

  SECTION("readBytes() all at once") {
    StreamStub src("ABC");
    Reader<StreamStub> reader(src);

    char buffer[8] = "abcd";
    REQUIRE(reader.readBytes(buffer, 4) == 3);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'd');
  }

  SECTION("readBytes() in two parts") {
    StreamStub src("ABCDEF");
    Reader<StreamStub> reader(src);

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
