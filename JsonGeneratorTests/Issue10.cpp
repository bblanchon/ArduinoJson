#include "CppUnitTest.h"
#include "JsonArray.h"
#include "JsonObject.h"

using namespace ArduinoJson::Generator;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{
    TEST_CLASS(Issue10)
    {
        struct Person {
            int id;
            char name[32];
        };

        Person persons[2];

    public:

        TEST_METHOD_INITIALIZE(Initialize)
        {
            Person boss;
            boss.id = 1;
            strcpy(boss.name, "Jeff");
            Person employee;
            employee.id = 2;
            strcpy(employee.name, "John");
            persons[0] = boss;
            persons[1] = employee;
        }

        TEST_METHOD(WrongWayToAddObjectInAnArray)
        {
            JsonArray<2> json;

            for (int i = 0; i < 2; i++)
            {
                JsonObject<2> object;
                
                object["id"] = persons[i].id;
                object["name"] = persons[i].name;

                json.add(object); // <- Adding a reference to a temporary variable
            }

            char buffer[256];
            json.printTo(buffer, sizeof(buffer));

            // the same values are repeated, that's normal
            Assert::AreEqual("[{\"id\":2,\"name\":\"John\"},{\"id\":2,\"name\":\"John\"}]", buffer);  
        }

        TEST_METHOD(RightWayToAddObjectInAnArray)
        {
            JsonArray<2> json;
            JsonObject<2> object[2];

            for (int i = 0; i < 2; i++) 
            {
                object[i]["id"] = persons[i].id;
                object[i]["name"] = persons[i].name;

                json.add(object[i]);
            }

            char buffer[256];
            json.printTo(buffer, sizeof(buffer));

            Assert::AreEqual("[{\"id\":1,\"name\":\"Jeff\"},{\"id\":2,\"name\":\"John\"}]", buffer);
        }
    };
}
