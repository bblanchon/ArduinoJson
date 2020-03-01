// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <stdlib.h>  // malloc, free
#include <catch.hpp>
#include <sstream>

using ARDUINOJSON_NAMESPACE::addPadding;
using ARDUINOJSON_NAMESPACE::move;

class SpyingAllocator {
 public:
  SpyingAllocator(const SpyingAllocator& src) : _log(src._log) {}
  SpyingAllocator(std::ostream& log) : _log(log) {}

  void* allocate(size_t n) {
    _log << "A" << n;
    return malloc(n);
  }
  void deallocate(void* p) {
    _log << (p ? "F" : "f");
    free(p);
  }

 private:
  SpyingAllocator& operator=(const SpyingAllocator& src);

  std::ostream& _log;
};

typedef BasicJsonDocument<SpyingAllocator> MyJsonDocument;

TEST_CASE("BasicJsonDocument") {
  std::stringstream log;

  SECTION("Construct/Destruct") {
    { MyJsonDocument doc(4096, log); }
    REQUIRE(log.str() == "A4096F");
  }

  SECTION("Copy construct") {
    {
      MyJsonDocument doc1(4096, log);
      doc1.set(std::string("The size of this string is 32!!"));

      MyJsonDocument doc2(doc1);

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
    }
    REQUIRE(log.str() == "A4096A32FF");
  }

  SECTION("Move construct") {
    {
      MyJsonDocument doc1(4096, log);
      doc1.set(std::string("The size of this string is 32!!"));

      MyJsonDocument doc2(move(doc1));

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
#if ARDUINOJSON_HAS_RVALUE_REFERENCES
      REQUIRE(doc1.as<std::string>() == "null");
      REQUIRE(doc1.capacity() == 0);
      REQUIRE(doc2.capacity() == 4096);
#endif
    }
#if ARDUINOJSON_HAS_RVALUE_REFERENCES
    REQUIRE(log.str() == "A4096Ff");
#else
    REQUIRE(log.str() == "A4096A32FF");
#endif
  }

  SECTION("Copy assign") {
    {
      MyJsonDocument doc1(4096, log);
      doc1.set(std::string("The size of this string is 32!!"));
      MyJsonDocument doc2(8, log);

      doc2 = doc1;

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
    }
    REQUIRE(log.str() == "A4096A8FA32FF");
  }

  SECTION("Move assign") {
    {
      MyJsonDocument doc1(4096, log);
      doc1.set(std::string("The size of this string is 32!!"));
      MyJsonDocument doc2(8, log);

      doc2 = move(doc1);

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
#if ARDUINOJSON_HAS_RVALUE_REFERENCES
      REQUIRE(doc1.as<std::string>() == "null");
      REQUIRE(doc1.capacity() == 0);
      REQUIRE(doc2.capacity() == 4096);
#endif
    }
#if ARDUINOJSON_HAS_RVALUE_REFERENCES
    REQUIRE(log.str() == "A4096A8FFf");
#else
    REQUIRE(log.str() == "A4096A8FA32FF");
#endif
  }
}
