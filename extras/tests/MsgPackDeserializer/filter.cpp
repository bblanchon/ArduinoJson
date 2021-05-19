// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("deserializeMsgPack() filter") {
  StaticJsonDocument<4096> doc;
  DeserializationError error;

  StaticJsonDocument<200> filter;
  DeserializationOption::Filter filterOpt(filter);

  SECTION("root is fixmap") {
    SECTION("filter = {include:true,ignore:false)") {
      filter["include"] = true;
      filter["ignore"] = false;

      SECTION("input truncated after ignored key") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore", 8, filterOpt);

        CHECK(error == DeserializationError::IncompleteInput);
        CHECK(doc.as<std::string>() == "{}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
      }

      SECTION("input truncated after inside skipped uint 8") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xCC\x2A\xA7include\x2A",
                                   9, filterOpt);

        CHECK(error == DeserializationError::IncompleteInput);
        CHECK(doc.as<std::string>() == "{}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
      }

      SECTION("input truncated after before skipped string size") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xd9", 9, filterOpt);

        CHECK(error == DeserializationError::IncompleteInput);
        CHECK(doc.as<std::string>() == "{}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
      }

      SECTION("input truncated after before skipped ext size") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xC7", 9, filterOpt);

        CHECK(error == DeserializationError::IncompleteInput);
        CHECK(doc.as<std::string>() == "{}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
      }

      SECTION("skip nil") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xC0\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("reject 0xc1") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xC1\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::InvalidInput);
      }

      SECTION("skip false") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xC2\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip true") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xC3\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip positive fixint") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\x2A\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip negative fixint") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xFF\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip uint 8") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xCC\x2A\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip int 8") {
        error = deserializeMsgPack(doc, "\x82\xA6ignore\xD0\x2A\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip uint 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xcd\x30\x39\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip int 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xD1\xCF\xC7\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip uint 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xCE\x12\x34\x56\x78\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip int 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xD2\xB6\x69\xFD\x2E\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip uint 64") {
        error = deserializeMsgPack(
            doc,
            "\x82\xA6ignore\xCF\x12\x34\x56\x78\x9A\xBC\xDE\xF0\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip int 64") {
        error = deserializeMsgPack(
            doc,
            "\x82\xA6ignore\xD3\x12\x34\x56\x78\x9A\xBC\xDE\xF0\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip float 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xCA\x40\x48\xF5\xC3\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip float 64") {
        error = deserializeMsgPack(
            doc,
            "\x82\xA6ignore\xCB\x40\x09\x21\xCA\xC0\x83\x12\x6F\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip fixstr") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xABhello world\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip str 8") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xd9\x05hello\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip str 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xda\x00\x05hello\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip str 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xdb\x00\x00\x00\x05hello\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip bin 8") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xC4\x05hello\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip bin 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xC5\x00\x05hello\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip bin 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xC6\x00\x00\x00\x05hello\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip fixarray") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\x92\x01\x02\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip array 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\xDC\x00\x02\xA5hello\xA5world\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip array 32") {
        error = deserializeMsgPack(
            doc,
            "\x82\xA6ignore"
            "\xDD\x00\x00\x00\x02\xCA\x00\x00\x00\x00\xCA\x40\x48\xF5\xC3"
            "\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip fixmap") {
        error = deserializeMsgPack(
            doc, "\x82\xA6ignore\x82\xA3one\x01\xA3two\x02\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip map 16") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xDE\x00\x02\xA1H\xA5hello\xA1W\xA5world"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip map 32") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xDF\x00\x00\x00\x02"
                                   "\xA4zero\xCA\x00\x00\x00\x00"
                                   "\xA2pi\xCA\x40\x48\xF5\xC3"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip fixext 1") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xd4\x01\x02"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip fixext 2") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xd5\x01\x02\x03"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip fixext 4") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xd6\x01\x02\x03\x04\x05"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip fixext 8") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xd7\x01\x02\x03\x04\x05\x06\x07\x08\x09"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip fixext 16") {
        error =
            deserializeMsgPack(doc,
                               "\x82\xA6ignore"
                               "\xd8\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A"
                               "\x0B\x0C\x0D\x0E\x0F\x10\x11"
                               "\xA7include\x2A",
                               filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip ext 8") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xc7\x02\x00\x01\x02"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip ext 16") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xc8\x00\x02\x00\x01\x02"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }

      SECTION("skip ext 32") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA6ignore"
                                   "\xc9\x00\x00\x00\x02\x00\x01\x02"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      }
    }

    SECTION("Filter = {arronly:[{measure:true}],include:true}") {
      filter["onlyarr"][0]["measure"] = true;
      filter["include"] = true;

      CAPTURE(filter.as<std::string>());

      SECTION("include fixarray") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr\x92"
                                   "\x82\xA8location\x01\xA7measure\x02"
                                   "\x82\xA8location\x02\xA7measure\x04"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() ==
              "{\"onlyarr\":[{\"measure\":2},{\"measure\":4}],\"include\":42}");
        CHECK(doc.memoryUsage() ==
              JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(2) + 24);
      }

      SECTION("include array 16") {
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
        CHECK(doc.memoryUsage() ==
              JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(2) + 24);
      }

      SECTION("include array 32") {
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
        CHECK(doc.memoryUsage() ==
              JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(2) + 24);
      }

      SECTION("skip null") {
        error = deserializeMsgPack(doc, "\x82\xA7onlyarr\xC0\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip false") {
        error = deserializeMsgPack(doc, "\x82\xA7onlyarr\xC2\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip true") {
        error = deserializeMsgPack(doc, "\x82\xA7onlyarr\xC3\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip positive fixint") {
        error = deserializeMsgPack(doc, "\x82\xA7onlyarr\x2A\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip negative fixint") {
        error = deserializeMsgPack(doc, "\x82\xA7onlyarr\xFF\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip uint 8") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xCC\x2A\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip uint 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xcd\x30\x39\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip uint 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xCE\x12\x34\x56\x78\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip uint 64") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr\xCF\x12\x34\x56\x78\x9A\xBC"
                                   "\xDE\xF0\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip int 8") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xD0\x2A\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip int 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xD1\xCF\xC7\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip int 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xD2\xB6\x69\xFD\x2E\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip int 64") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr\xD3\x12\x34\x56\x78\x9A\xBC"
                                   "\xDE\xF0\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip float 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xCA\x40\x48\xF5\xC3\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip float 64") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr\xCB\x40\x09\x21\xCA\xC0\x83"
                                   "\x12\x6F\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip fixstr") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xABhello world\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip str 8") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xd9\x05hello\xA7include\x2A", filterOpt);

        CHECK(error == DeserializationError::Ok);
      }

      SECTION("skip str 16") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xda\x00\x05hello\xA7include\x2A", filterOpt);

        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
      }

      SECTION("skip str 32") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\xdb\x00\x00\x00\x05hello\xA7include\x2A",
            filterOpt);

        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip fixmap") {
        error = deserializeMsgPack(
            doc, "\x82\xA7onlyarr\x82\xA3one\x01\xA3two\x02\xA7include\x2A",
            filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip map 16") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr"
                                   "\xDE\x00\x02\xA1H\xA5hello\xA1W\xA5world"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }

      SECTION("skip map 32") {
        error = deserializeMsgPack(doc,
                                   "\x82\xA7onlyarr"
                                   "\xDF\x00\x00\x00\x02"
                                   "\xA4zero\xCA\x00\x00\x00\x00"
                                   "\xA2pi\xCA\x40\x48\xF5\xC3"
                                   "\xA7include\x2A",
                                   filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "{\"onlyarr\":null,\"include\":42}");
        CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      }
    }
  }

  SECTION("root is fixarray") {
    SECTION("filter = [false, true]") {
      filter[0] = false;  // only the first elment of the filter matters
      filter[1] = true;   // so this one is ignored

      SECTION("input = [1,2,3]") {
        error = deserializeMsgPack(doc, "\x93\x01\x02\x03", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "[]");
        CHECK(doc.memoryUsage() == JSON_ARRAY_SIZE(0));
      }
    }

    SECTION("filter = [true, false]") {
      filter[0] = true;   // only the first elment of the filter matters
      filter[1] = false;  // so this one is ignored

      SECTION("input = [1,2,3]") {
        error = deserializeMsgPack(doc, "\x93\x01\x02\x03", filterOpt);

        CHECK(error == DeserializationError::Ok);
        CHECK(doc.as<std::string>() == "[1,2,3]");
        CHECK(doc.memoryUsage() == JSON_ARRAY_SIZE(3));
      }
    }
  }

  SECTION("Filter = {onlyobj:{measure:true},include:true}") {
    filter["onlyobj"]["measure"] = true;
    filter["include"] = true;

    CAPTURE(filter.as<std::string>());

    SECTION("include fixmap") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj"
                                 "\x82\xA8location\x01\xA7measure\x02"
                                 "\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() ==
            "{\"onlyobj\":{\"measure\":2},\"include\":42}");
      CHECK(doc.memoryUsage() ==
            JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(1) + 24);
    }

    SECTION("include map 16") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj"
                                 "\xDE\x00\x02\xA8location\x01\xA7measure\x02"
                                 "\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() ==
            "{\"onlyobj\":{\"measure\":2},\"include\":42}");
      CHECK(doc.memoryUsage() ==
            JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(1) + 24);
    }

    SECTION("include map 32") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj"
                                 "\xDF\x00\x00\x00\x02"
                                 "\xA8location\x01\xA7measure\x02"
                                 "\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() ==
            "{\"onlyobj\":{\"measure\":2},\"include\":42}");
      CHECK(doc.memoryUsage() ==
            JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(1) + 24);
    }

    SECTION("skip null") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xC0\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip false") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xC2\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip true") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xC3\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip positive fixint") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\x2A\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip negative fixint") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xFF\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip uint 8") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xCC\x2A\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip uint 16") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xcd\x30\x39\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip uint 32") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xCE\x12\x34\x56\x78\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip uint 64") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj\xCF\x12\x34\x56\x78\x9A\xBC"
                                 "\xDE\xF0\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip int 8") {
      error = deserializeMsgPack(doc, "\x82\xA7onlyobj\xD0\x2A\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip int 16") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xD1\xCF\xC7\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip int 32") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xD2\xB6\x69\xFD\x2E\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip int 64") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj\xD3\x12\x34\x56\x78\x9A\xBC"
                                 "\xDE\xF0\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip float 32") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xCA\x40\x48\xF5\xC3\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip float 64") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj\xCB\x40\x09\x21\xCA\xC0\x83"
                                 "\x12\x6F\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip fixstr") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xABhello world\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip str 8") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xd9\x05hello\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
    }

    SECTION("skip str 16") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xda\x00\x05hello\xA7include\x2A", filterOpt);

      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
    }

    SECTION("skip str 32") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\xdb\x00\x00\x00\x05hello\xA7include\x2A",
          filterOpt);

      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip fixarray") {
      error = deserializeMsgPack(
          doc, "\x82\xA7onlyobj\x92\x01\x02\xA7include\x2A", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip array 16") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj\xDC\x00\x01\xA7"
                                 "example\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }

    SECTION("skip array 32") {
      error = deserializeMsgPack(doc,
                                 "\x82\xA7onlyobj"
                                 "\xDD\x00\x00\x00\x02\x01\x02"
                                 "\xA7include\x2A",
                                 filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.as<std::string>() == "{\"onlyobj\":null,\"include\":42}");
      CHECK(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
    }
  }

  SECTION("filter = true") {
    filter.set(true);

    error = deserializeMsgPack(doc, "\x90", filterOpt);

    CHECK(error == DeserializationError::Ok);
    CHECK(doc.is<JsonArray>() == true);
    CHECK(doc.size() == 0);
  }

  SECTION("filter = false") {
    filter.set(false);

    SECTION("input = fixarray") {
      error = deserializeMsgPack(doc, "\x92\x01\x02", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.isNull() == true);
    }

    SECTION("input = array 16") {
      error = deserializeMsgPack(doc, "\xDC\x00\x02\x01\x02", filterOpt);

      CHECK(error == DeserializationError::Ok);
      CHECK(doc.isNull() == true);
    }

    SECTION("array too deep") {
      error = deserializeMsgPack(doc, "\x91\x91\x91\x91\x91", 5, filterOpt,
                                 DeserializationOption::NestingLimit(4));

      CHECK(error == DeserializationError::TooDeep);
    }

    SECTION("object too deep") {
      error = deserializeMsgPack(
          doc, "\x81\xA1z\x81\xA1z\x81\xA1z\x81\xA1z\x81\xA1z", 15, filterOpt,
          DeserializationOption::NestingLimit(4));

      CHECK(error == DeserializationError::TooDeep);
    }
  }
}

TEST_CASE("Overloads") {
  StaticJsonDocument<256> doc;
  StaticJsonDocument<256> filter;

  using namespace DeserializationOption;

  // deserializeMsgPack(..., Filter)

  SECTION("const char*, Filter") {
    deserializeMsgPack(doc, "{}", Filter(filter));
  }

  SECTION("const char*, size_t, Filter") {
    deserializeMsgPack(doc, "{}", 2, Filter(filter));
  }

  SECTION("const std::string&, Filter") {
    deserializeMsgPack(doc, std::string("{}"), Filter(filter));
  }

  SECTION("std::istream&, Filter") {
    std::stringstream s("{}");
    deserializeMsgPack(doc, s, Filter(filter));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("char[n], Filter") {
    int i = 4;
    char vla[i];
    strcpy(vla, "{}");
    deserializeMsgPack(doc, vla, Filter(filter));
  }
#endif

  // deserializeMsgPack(..., Filter, NestingLimit)

  SECTION("const char*, Filter, NestingLimit") {
    deserializeMsgPack(doc, "{}", Filter(filter), NestingLimit(5));
  }

  SECTION("const char*, size_t, Filter, NestingLimit") {
    deserializeMsgPack(doc, "{}", 2, Filter(filter), NestingLimit(5));
  }

  SECTION("const std::string&, Filter, NestingLimit") {
    deserializeMsgPack(doc, std::string("{}"), Filter(filter), NestingLimit(5));
  }

  SECTION("std::istream&, Filter, NestingLimit") {
    std::stringstream s("{}");
    deserializeMsgPack(doc, s, Filter(filter), NestingLimit(5));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("char[n], Filter, NestingLimit") {
    int i = 4;
    char vla[i];
    strcpy(vla, "{}");
    deserializeMsgPack(doc, vla, Filter(filter), NestingLimit(5));
  }
#endif

  // deserializeMsgPack(..., NestingLimit, Filter)

  SECTION("const char*, NestingLimit, Filter") {
    deserializeMsgPack(doc, "{}", NestingLimit(5), Filter(filter));
  }

  SECTION("const char*, size_t, NestingLimit, Filter") {
    deserializeMsgPack(doc, "{}", 2, NestingLimit(5), Filter(filter));
  }

  SECTION("const std::string&, NestingLimit, Filter") {
    deserializeMsgPack(doc, std::string("{}"), NestingLimit(5), Filter(filter));
  }

  SECTION("std::istream&, NestingLimit, Filter") {
    std::stringstream s("{}");
    deserializeMsgPack(doc, s, NestingLimit(5), Filter(filter));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("char[n], NestingLimit, Filter") {
    int i = 4;
    char vla[i];
    strcpy(vla, "{}");
    deserializeMsgPack(doc, vla, NestingLimit(5), Filter(filter));
  }
#endif
}
