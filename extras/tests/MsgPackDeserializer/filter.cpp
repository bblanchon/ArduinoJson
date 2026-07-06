// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include <sstream>

#include "Allocators.hpp"
#include "Literals.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("deserializeMsgPack() filter") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  DeserializationError error;

  JsonDocument filter;
  DeserializationOption::Filter filterOpt(filter);

  SUBCASE("root is fixmap") {
    SUBCASE("filter = {include:true,ignore:false)") {
      filter["include"] = true;
      filter["ignore"] = false;

      SUBCASE("input truncated after ignored key") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore", 8, filterOpt);

        CHECK(error == DeserializationError::IncompleteInput);
        CHECK(doc.as<std::string>() == "{}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                           });
      }

      SUBCASE("input truncated after inside skipped uint 8") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xCC\x2A\xA7include\x2A",
                                   9, filterOpt);

        CHECK(error == DeserializationError::IncompleteInput);
        CHECK(doc.as<std::string>() == "{}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                           });
      }

      SUBCASE("input truncated after before skipped string size") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xd9", 9, filterOpt);

        CHECK(error == DeserializationError::IncompleteInput);
        CHECK(doc.as<std::string>() == "{}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                           });
      }

      SUBCASE("input truncated after before skipped ext size") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xC7", 9, filterOpt);

        CHECK(error == DeserializationError::IncompleteInput);
        CHECK(doc.as<std::string>() == "{}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                           });
      }

      SUBCASE("skip nil") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xC0\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("reject 0xc1") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xC1\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::InvalidInput);
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                           });
      }

      SUBCASE("skip false") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xC2\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip true") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xC3\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip positive fixint") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\x2A\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip negative fixint") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xFF\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip uint 8") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xCC\x2A\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip int 8") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xD0\x2A\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip uint 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xcd\x30\x39\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip int 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xD1\xCF\xC7\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip uint 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xCE\x12\x34\x56\x78\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip int 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xD2\xB6\x69\xFD\x2E\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip uint 64") {
        error = deserializeMsgPack(
            doc,
            "\x82\xA6ignore\xCF\x12\x34\x56\x78\x9A\xBC\xDE\xF0\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip int 64") {
        error = deserializeMsgPack(
            doc,
            "\x82\xA6ignore\xD3\x12\x34\x56\x78\x9A\xBC\xDE\xF0\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip float 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xCA\x40\x48\xF5\xC3\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip float 64") {
        error = deserializeMsgPack(
            doc,
            "\x82\xA6ignore\xCB\x40\x09\x21\xCA\xC0\x83\x12\x6F\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip fixstr") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xABhello world\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip str 8") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xd9\x05hello\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip str 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xda\x00\x05hello\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip str 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xdb\x00\x00\x00\x05hello\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip bin 8") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xC4\x05hello\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip bin 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xC5\x00\x05hello\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip bin 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xC6\x00\x00\x00\x05hello\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip fixarray") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\x92\x01\x02\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip array 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xDC\x00\x02\xA5hello\xA5world\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip array 32") {
        error = deserializeMsgPack(
            doc,
            "\x82\xA6ignore"
            "\xDD\x00\x00\x00\x02\xCA\x00\x00\x00\x00\xCA\x40\x48\xF5\xC3"
            "\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip fixmap") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\x82\xA3one\x01\xA3two\x02\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip map 16") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xDE\x00\x02\xA1H\xA5hello\xA1W\xA5world"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip map 32") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xDF\x00\x00\x00\x02"
                                   "\xA4zero\xCA\x00\x00\x00\x00"
                                   "\xA2pi\xCA\x40\x48\xF5\xC3"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip fixext 1") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xd4\x01\x02"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip fixext 2") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xd5\x01\x02\x03"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip fixext 4") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xd6\x01\x02\x03\x04\x05"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip fixext 8") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xd7\x01\x02\x03\x04\x05\x06\x07\x08\x09"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip fixext 16") {
        error =
            deserializeMsgPack(doc,
                               "\x82\xA6ignore"
                               "\xd8\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A"
                               "\x0B\x0C\x0D\x0E\x0F\x10\x11"
                               "\xA7include\x2A",
                               filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip ext 8") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xc7\x02\x00\x01\x02"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip ext 16") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xc8\x00\x02\x00\x01\x02"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }

      SUBCASE("skip ext 32") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xc9\x00\x00\x00\x02\x00\x01\x02"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("ignore")),
                               Deallocate(sizeofString("ignore")),
                               Allocate(sizeofString("include")),
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofObject(1)),
                           });
      }
    }

    SUBCASE("Filter = {arronly:[{measure:true}],include:true}") {
      filter["onlyarr"][0]["measure"] = true;
      filter["include"] = true;

      CAPTURE(filter.as<std::string>());

      SUBCASE("include fixarray") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr\x92"
                                   "\x82\xA8location\x01\xA7measure\x02"
                                   "\x82\xA8location\x02\xA7measure\x04"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() ==
              "{\"onlyarr\":[{\"measure\":2},{\"measure\":4}],\"include\":42}");
        CHECK(spy.log() ==
              AllocatorLog{
                  Allocate(sizeofString("onlyarr")),
                  Allocate(sizeofPool()),
                  Allocate(sizeofString("location")),
                  Reallocate(sizeofString("location"), sizeofString("measure")),
                  Allocate(sizeofString("location")),
                  Reallocate(sizeofString("location"), sizeofString("include")),
                  Reallocate(sizeofPool(), sizeofObject(2) + sizeofArray(2) +
                                               2 * sizeofObject(1)),
              });
      }

      SUBCASE("include array 16") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr"
                                   "\xDC\x00\x02"
                                   "\x82\xA8location\x01\xA7measure\x02"
                                   "\x82\xA8location\x02\xA7measure\x04"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() ==
              "{\"onlyarr\":[{\"measure\":2},{\"measure\":4}],\"include\":42}");
        CHECK(spy.log() ==
              AllocatorLog{
                  Allocate(sizeofString("onlyarr")),
                  Allocate(sizeofPool()),
                  Allocate(sizeofString("location")),
                  Reallocate(sizeofString("location"), sizeofString("measure")),
                  Allocate(sizeofString("location")),
                  Reallocate(sizeofString("location"), sizeofString("include")),
                  Reallocate(sizeofPool(), sizeofObject(2) + sizeofArray(2) +
                                               2 * sizeofObject(1)),
              });
      }

      SUBCASE("include array 32") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr"
                                   "\xDD\x00\x00\x00\x02"
                                   "\x82\xA8location\x01\xA7measure\x02"
                                   "\x82\xA8location\x02\xA7measure\x04"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() ==
              "{\"onlyarr\":[{\"measure\":2},{\"measure\":4}],\"include\":42}");
        CHECK(spy.log() ==
              AllocatorLog{
                  Allocate(sizeofString("onlyarr")),
                  Allocate(sizeofPool()),
                  Allocate(sizeofString("location")),
                  Reallocate(sizeofString("location"), sizeofString("measure")),
                  Allocate(sizeofString("location")),
                  Reallocate(sizeofString("location"), sizeofString("include")),
                  Reallocate(sizeofPool(), sizeofObject(2) + sizeofArray(2) +
                                               2 * sizeofObject(1)),
              });
      }

      SUBCASE("skip null") {
        error = deserializeMsgPack(doc, "\x82\xA7onlyarr\xC0\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip false") {
        error = deserializeMsgPack(doc, "\x82\xA7onlyarr\xC2\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip true") {
        error = deserializeMsgPack(doc, "\x82\xA7onlyarr\xC3\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip positive fixint") {
        error = deserializeMsgPack(doc, "\x82\xA7onlyarr\x2A\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip negative fixint") {
        error = deserializeMsgPack(doc, "\x82\xA7onlyarr\xFF\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip uint 8") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xCC\x2A\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip uint 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xcd\x30\x39\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip uint 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xCE\x12\x34\x56\x78\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip uint 64") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr\xCF\x12\x34\x56\x78\x9A\xBC"
                                   "\xDE\xF0\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip int 8") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xD0\x2A\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip int 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xD1\xCF\xC7\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip int 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xD2\xB6\x69\xFD\x2E\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip int 64") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr\xD3\x12\x34\x56\x78\x9A\xBC"
                                   "\xDE\xF0\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip float 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xCA\x40\x48\xF5\xC3\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip float 64") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr\xCB\x40\x09\x21\xCA\xC0\x83"
                                   "\x12\x6F\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip fixstr") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xABhello world\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip str 8") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xd9\x05hello\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
      }

      SUBCASE("skip str 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xda\x00\x05hello\xA7include\x2A", filterOpt);

        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
      }

      SUBCASE("skip str 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xdb\x00\x00\x00\x05hello\xA7include\x2A",
            filterOpt);

        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip fixmap") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\x82\xA3one\x01\xA3two\x02\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("one")),
                               Deallocate(sizeofString("one")),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip map 16") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr"
                                   "\xDE\x00\x02\xA1H\xA5hello\xA1W\xA5world"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("H")),
                               Deallocate(sizeofString("H")),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }

      SUBCASE("skip map 32") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr"
                                   "\xDF\x00\x00\x00\x02"
                                   "\xA4zero\xCA\x00\x00\x00\x00"
                                   "\xA2pi\xCA\x40\x48\xF5\xC3"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofString("onlyarr")),
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("zero")),
                               Deallocate(sizeofString("zero")),
                               Allocate(sizeofString("include")),
                               Reallocate(sizeofPool(), sizeofObject(2)),
                           });
      }
    }
  }

  SUBCASE("root is fixarray") {
    SUBCASE("filter = [false, true]") {
      filter[0] = false;  // only the first elment of the filter matters
      filter[1] = true;   // so this one is ignored

      SUBCASE("input = [1,2,3]") {
        error = deserializeMsgPack(doc, "\x93\x01\x02\x03", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "[]");
        CHECK(spy.log() == AllocatorLog());
      }
    }

    SUBCASE("filter = [true, false]") {
      filter[0] = true;   // only the first elment of the filter matters
      filter[1] = false;  // so this one is ignored

      SUBCASE("input = [1,2,3]") {
        error = deserializeMsgPack(doc, "\x93\x01\x02\x03", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "[1,2,3]");
        CHECK(spy.log() == AllocatorLog{
                               Allocate(sizeofPool()),
                               Reallocate(sizeofPool(), sizeofArray(3)),
                           });
      }
    }
  }

  SUBCASE("Filter = {onlyobj:{measure:true},include:true}") {
    filter["onlyobj"]["measure"] = true;
    filter["include"] = true;

    CAPTURE(filter.as<std::string>());

    SUBCASE("include fixmap") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj"
                                 "\x82\xA8location\x01\xA7measure\x02"
                                 "\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() ==
            "{\"onlyobj\":{\"measure\":2},\"include\":42}");
      CHECK(spy.log() ==
            AllocatorLog{
                Allocate(sizeofString("onlyobj")),
                Allocate(sizeofPool()),
                Allocate(sizeofString("location")),
                Reallocate(sizeofString("location"), sizeofString("measure")),
                Allocate(sizeofString("include")),
                Reallocate(sizeofPool(), sizeofObject(2) + sizeofObject(1)),
            });
    }

    SUBCASE("include map 16") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj"
                                 "\xDE\x00\x02\xA8location\x01\xA7measure\x02"
                                 "\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() ==
            "{\"onlyobj\":{\"measure\":2},\"include\":42}");
      CHECK(spy.log() ==
            AllocatorLog{
                Allocate(sizeofString("onlyobj")),
                Allocate(sizeofPool()),
                Allocate(sizeofString("location")),
                Reallocate(sizeofString("location"), sizeofString("measure")),
                Allocate(sizeofString("include")),
                Reallocate(sizeofPool(), sizeofObject(2) + sizeofObject(1)),
            });
    }

    SUBCASE("include map 32") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj"
                                 "\xDF\x00\x00\x00\x02"
                                 "\xA8location\x01\xA7measure\x02"
                                 "\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() ==
            "{\"onlyobj\":{\"measure\":2},\"include\":42}");
      CHECK(spy.log() ==
            AllocatorLog{
                Allocate(sizeofString("onlyobj")),
                Allocate(sizeofPool()),
                Allocate(sizeofString("location")),
                Reallocate(sizeofString("location"), sizeofString("measure")),
                Allocate(sizeofString("include")),
                Reallocate(sizeofPool(), sizeofObject(2) + sizeofObject(1)),
            });
    }

    SUBCASE("skip null") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xC0\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip false") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xC2\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip true") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xC3\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip positive fixint") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\x2A\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip negative fixint") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xFF\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip uint 8") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xCC\x2A\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip uint 16") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xcd\x30\x39\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip uint 32") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xCE\x12\x34\x56\x78\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip uint 64") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj\xCF\x12\x34\x56\x78\x9A\xBC"
                                 "\xDE\xF0\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip int 8") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xD0\x2A\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip int 16") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xD1\xCF\xC7\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip int 32") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xD2\xB6\x69\xFD\x2E\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip int 64") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj\xD3\x12\x34\x56\x78\x9A\xBC"
                                 "\xDE\xF0\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip float 32") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xCA\x40\x48\xF5\xC3\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip float 64") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj\xCB\x40\x09\x21\xCA\xC0\x83"
                                 "\x12\x6F\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip fixstr") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xABhello world\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip str 8") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xd9\x05hello\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
    }

    SUBCASE("skip str 16") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xda\x00\x05hello\xA7include\x2A", filterOpt);

      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
    }

    SUBCASE("skip str 32") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xdb\x00\x00\x00\x05hello\xA7include\x2A",
          filterOpt);

      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip fixarray") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\x92\x01\x02\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip array 16") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj\xDC\x00\x01\xA7"
                                 "example\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }

    SUBCASE("skip array 32") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj"
                                 "\xDD\x00\x00\x00\x02\x01\x02"
                                 "\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(spy.log() == AllocatorLog{
                             Allocate(sizeofString("onlyarr")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("include")),
                             Reallocate(sizeofPool(), sizeofObject(2)),
                         });
    }
  }

  SUBCASE("filter = true") {
    filter.set(true);

    error = deserializeMsgPack(doc, "\x90", filterOpt);

    CHECK(error == DeserializationError::Ok);
    CHECK(doc.is<JsonArray>() == true);
    CHECK(doc.size() == 0);
  }

  SUBCASE("filter = false") {
    filter.set(false);

    SUBCASE("input = fixarray") {
      error = deserializeMsgPack(doc, "\x92\x01\x02", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.isNull() == true);
    }

    SUBCASE("input = array 16") {
      error = deserializeMsgPack(doc, "\xDC\x00\x02\x01\x02", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.isNull() == true);
    }

    SUBCASE("array too deep") {
      error = deserializeMsgPack(doc, "\x91\x91\x91\x91\x91", 5, filterOpt,
                                 DeserializationOption::NestingLimit(4));

      CHECK(error == DeserializationError::TooDeep);
    }

    SUBCASE("object too deep") {
      error = deserializeMsgPack(
          doc, "\x81\xA1z\x81\xA1z\x81\xA1z\x81\xA1z\x81\xA1z", 15, filterOpt,
          DeserializationOption::NestingLimit(4));

      CHECK(error == DeserializationError::TooDeep);
    }
  }
}

TEST_CASE("Zero-copy mode") {  // issue #1697
  char input[] = "\x82\xA7include\x01\xA6ignore\x02";

  JsonDocument filter;
  filter["include"] = true;

  JsonDocument doc;
  DeserializationError err =
      deserializeMsgPack(doc, input, 18, DeserializationOption::Filter(filter));

  CHECK(err == DeserializationError::Ok);
  CHECK(doc.as<std::string>() == "{\"include\":1}");
}

TEST_CASE("Overloads") {
  JsonDocument doc;
  JsonDocument filter;

  using namespace DeserializationOption;

  // deserializeMsgPack(..., Filter)

  SUBCASE("const char*, Filter") {
    deserializeMsgPack(doc, "{}", Filter(filter));
  }

  SUBCASE("const char*, size_t, Filter") {
    deserializeMsgPack(doc, "{}", 2, Filter(filter));
  }

  SUBCASE("const std::string&, Filter") {
    deserializeMsgPack(doc, "{}"_s, Filter(filter));
  }

  SUBCASE("std::istream&, Filter") {
    std::stringstream s("{}");
    deserializeMsgPack(doc, s, Filter(filter));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("char[n], Filter") {
    size_t i = 4;
    char vla[i];
    strcpy(vla, "{}");
    deserializeMsgPack(doc, vla, Filter(filter));
  }
#endif

  // deserializeMsgPack(..., Filter, NestingLimit)

  SUBCASE("const char*, Filter, NestingLimit") {
    deserializeMsgPack(doc, "{}", Filter(filter), NestingLimit(5));
  }

  SUBCASE("const char*, size_t, Filter, NestingLimit") {
    deserializeMsgPack(doc, "{}", 2, Filter(filter), NestingLimit(5));
  }

  SUBCASE("const std::string&, Filter, NestingLimit") {
    deserializeMsgPack(doc, "{}"_s, Filter(filter), NestingLimit(5));
  }

  SUBCASE("std::istream&, Filter, NestingLimit") {
    std::stringstream s("{}");
    deserializeMsgPack(doc, s, Filter(filter), NestingLimit(5));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("char[n], Filter, NestingLimit") {
    size_t i = 4;
    char vla[i];
    strcpy(vla, "{}");
    deserializeMsgPack(doc, vla, Filter(filter), NestingLimit(5));
  }
#endif

  // deserializeMsgPack(..., NestingLimit, Filter)

  SUBCASE("const char*, NestingLimit, Filter") {
    deserializeMsgPack(doc, "{}", NestingLimit(5), Filter(filter));
  }

  SUBCASE("const char*, size_t, NestingLimit, Filter") {
    deserializeMsgPack(doc, "{}", 2, NestingLimit(5), Filter(filter));
  }

  SUBCASE("const std::string&, NestingLimit, Filter") {
    deserializeMsgPack(doc, "{}"_s, NestingLimit(5), Filter(filter));
  }

  SUBCASE("std::istream&, NestingLimit, Filter") {
    std::stringstream s("{}");
    deserializeMsgPack(doc, s, NestingLimit(5), Filter(filter));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("char[n], NestingLimit, Filter") {
    size_t i = 4;
    char vla[i];
    strcpy(vla, "{}");
    deserializeMsgPack(doc, vla, NestingLimit(5), Filter(filter));
  }
#endif
}
