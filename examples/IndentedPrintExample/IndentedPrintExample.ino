/*
 * Arduino JSON library - IndentedPrint example
 * Benoit Blanchon 2014 - MIT License
 */

#include <JsonGenerator.h>

using namespace ArduinoJson::Generator;

void setup()
{
    Serial.begin(9600);
    
    JsonObject<1> json;
    json["key"] = "value";
    
    IndentedPrint serial(Serial);    
    serial.setTabSize(4);
    
    serial.println("This is at indentation 0");
    serial.indent();
    serial.println("This is at indentation 1");
    serial.println("This is also at indentation 1");
    serial.indent();
    serial.println("This is at indentation 2");
    
    serial.println("You can print JSON here, as usual:");
    serial.println(json);
    serial.println();
        
    serial.println("But you can also prettyPrint JSON here:");
    json.prettyPrintTo(serial);  
    serial.println();
    
    serial.unindent();
    serial.unindent();
    serial.println("This is back at indentation 0");
}

void loop()
{

}