// Example usage for ArduinoJson library by Benoit Blanchon <blog.benoitblanchon.fr>.

#include "arduinoJson.h"

// Initialize objects from the lib
ArduinoJson arduinoJson;

void setup() {
    // Call functions on initialized library objects that require hardware
    arduinoJson.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    arduinoJson.process();
}
