// Copyright Steve Nelson 2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/


// About
// -----
// This example shows how to use the ArduinoJson library in a class file
// Using class files is an underused technique in Arduino code.
// ArduinoJson is a very effective way to store complex data inside of a class file
//


#include "users.h"

//This creates the global object variable "users" which is used below to access the
//public methods which exist in users.h
Users users;

void setup() {
  Serial.begin(115200);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[BOOTING] WAIT %d...\n", t);
    Serial.flush();
    delay(50);
  }


  //This is calling the saveUser() user method
  //Notice how I am first assigning the data to
  //local char[] variables.
  Serial.println("Add a user");
  Serial.println("------------------");
  char userId[]="1234";
  String firstName="steve";
  users.saveUser(userId,firstName);
  Serial.println(users.getUsers(false));


  //This is the same thing as above but I'm specifying the
  //datatype in the method call itself.
  Serial.println("Add another user");
  Serial.println("------------------");
  users.saveUser((char*)"5678","nicole");
  Serial.println(users.getUsers(false));


  //This one will still work but you'll see a compiler warning
  //because It's not specifying the (char*) datatype for the userId variable
  //the compiler is guessing it is a string variable.
  //If you put (char*) in front of the "2345" it will fix the warning
  Serial.println("Add a third user");
  Serial.println("------------------");
  users.saveUser("2345","zoe");
  Serial.println(users.getUsers(false));


  
  Serial.println("Delete a user");
  Serial.println("------------------");
  users.deleteUser((char*)"1234");
  Serial.println(users.getUsers(true));

}

void loop() {
  // put your main code here, to run repeatedly:

}
