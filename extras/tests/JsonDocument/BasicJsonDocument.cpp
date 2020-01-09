// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <stdlib.h>  // malloc, free
#include <catch.hpp>
#include <sstream>

using ARDUINOJSON_NAMESPACE::addPadding;

class SpyingAllocator {
 public:
  SpyingAllocator(std::ostream& log) : _log(log) {}

  void* allocate(size_t n) {
    _log << "A" << n;
    return malloc(n);
  }
  void deallocate(void* p) {
    _log << "F";
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
    }
    REQUIRE(log.str() == "A4096A32FF");
  }
}
