

#include <ArduinoJson.h>
#include <stdint.h>
// Messagepack vector generated using online tool ( https://kawanet.github.io/msgpack-lite/ )
uint8_t testVector[] = {133, 161, 84, 203, 192, 2, 102, 102, 96, 0, 0, 0, 161, 67, 210, 255, 254, 12, 0, 161, 68, 146, 194, 195, 161, 69, 132, 165, 68, 97, 116, 97, 49, 2, 165, 68, 97, 116, 97, 50, 205, 4, 5, 165, 68, 97, 116, 97, 51, 210, 255, 254, 255, 255, 165, 68, 97, 116, 97, 52, 166, 83, 116, 114, 105, 110, 103, 161, 70, 146, 165, 99, 104, 97, 114, 49, 165, 99, 104, 97, 114, 50};

float32_t s;

void setup()
{
  Serial.begin(115200);
}

void loop() {
  char JsonText[256] = {0};

  DynamicJsonBuffer jsonBuffer;

  JsonObject &exampleSerializer = jsonBuffer.createObject();

  Serial.println("--------------------------");
  Serial.println("Creating Json structure...");

  exampleSerializer["T"] = -2.3;
  exampleSerializer["C"] = -128000;

  JsonArray &data1 = jsonBuffer.createArray();
  data1.add(false);
  data1.add(true);
  exampleSerializer["D"] = data1;
  exampleSerializer["E"] = jsonBuffer.createObject();
  exampleSerializer["E"]["Data1"] = 2;
  exampleSerializer["E"]["Data2"] = 1029;
  exampleSerializer["E"]["Data3"] = -65537;
  exampleSerializer["E"]["Data4"] = "String";

  JsonArray &data2 = jsonBuffer.createArray();
  data2.add("char1");
  data2.add("char2");
  exampleSerializer["F"] = data2;

  size_t size = exampleSerializer.printTo(JsonText, sizeof(JsonText));

  Serial.println("Serialized JSON:");
  Serial.println(JsonText);

  size = exampleSerializer.binaryPrintTo(JsonText, sizeof(JsonText));

  Serial.println("Serialized MessagePack:");
  for (uint8_t i = 0; i < size; i++)
  {
    Serial.print((uint8_t)JsonText[i], HEX);
    Serial.write(' ');
  }
  
  Serial.println("\nAccess https://kawanet.github.io/msgpack-lite/ to verify the HEX output");

  Serial.println("--------------------------------");
  Serial.println("Reading above MessagePack structure...");

  //JsonObject &exampleDeserializer = jsonBuffer.parseBinaryObject(testVector); //use binary test vector
  JsonObject &exampleDeserializer = jsonBuffer.parseBinaryObject(JsonText);

  //Reading values from deserialized binary vector
  Serial.println("T: " + String((float)exampleDeserializer["T"]));
  Serial.println("C: " + String((int32_t)exampleDeserializer["C"]));
  Serial.println("D[0]: " + String(((bool)exampleDeserializer["D"][0] ? "true" : "false")));
  Serial.println("D[1]: " + String(((bool)exampleDeserializer["D"][1] ? "true" : "false")));
  Serial.println("E[\"Data1\"]: " + String((int)exampleDeserializer["E"]["Data1"]));
  Serial.println("E[\"Data2\"]: " + String((int)exampleDeserializer["E"]["Data2"]));
  Serial.println("E[\"Data3\"]: " + String((int32_t)exampleDeserializer["E"]["Data3"]));
  Serial.println("E[\"Data4\"]: " + String((const char *)exampleDeserializer["E"]["Data4"]));
  Serial.println("F[\"char1\"]: " + String((const char *)exampleDeserializer["F"][0] ));
  Serial.println("F[\"char2\"]: " + String((const char *)exampleDeserializer["F"][1] ));

  Serial.println( "Example finished!");
  Serial.println("--------------------------------");

  delay(10000);
}
