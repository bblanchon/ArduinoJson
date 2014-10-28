// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/JsonArray.hpp>
#include <ArduinoJson/JsonObject.hpp>
#include <ArduinoJson/JsonValue.hpp>
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

struct Person {
  int id;
  char name[32];
};

class Issue10 : public testing::Test {
 protected:
  virtual void SetUp() {
    Person boss;
    boss.id = 1;
    strcpy(boss.name, "Jeff");
    Person employee;
    employee.id = 2;
    strcpy(employee.name, "John");
    persons[0] = boss;
    persons[1] = employee;
  }

  void checkJsonString(JsonPrintable &p) {
    char buffer[256];
    p.printTo(buffer, sizeof(buffer));

    EXPECT_STREQ("[{\"id\":1,\"name\":\"Jeff\"},{\"id\":2,\"name\":\"John\"}]",
                 buffer);
  }

  Person persons[2];
};

TEST_F(Issue10, PopulateArrayByAddingAnObject) {
  StaticJsonBuffer<200> json;
  JsonArray array = json.createArray();

  for (int i = 0; i < 2; i++) {
    JsonObject object = json.createObject();

    object["id"] = persons[i].id;
    object["name"] = persons[i].name;

    array.add(object);  // <- adds a reference to an existing objet (creates 2
                        // extra proxy nodes)
  }

  checkJsonString(array);
}

TEST_F(Issue10, PopulateArrayByCreatingNestedObjects) {
  StaticJsonBuffer<200> json;
  JsonArray array = json.createArray();

  for (int i = 0; i < 2; i++) {
    JsonObject object = array.createNestedObject();

    object["id"] = persons[i].id;
    object["name"] = persons[i].name;
  }

  checkJsonString(array);
}
