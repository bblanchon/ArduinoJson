//These two lines check to see if users.h has been included already.
#ifndef _USERS_H_
#define _USERS_H_


#include <ArduinoJson.h>


//The name of the class is the new datatype we're creating
class Users
{
  //public methods can be called in the .ino file
  public:
    Users();
    void saveUser(char userId[], String firstName);
    void deleteUser(char userId[]);
    String getUsers(bool prettyPrint = false);

  //private variables cannot be accessed outside of this .h file
  private:
    DynamicJsonBuffer _jsonBuffer;
    JsonObject& _userData = _jsonBuffer.createObject();
};

Users::Users(){
  
}
String Users::getUsers(bool prettyPrint){
  String userJson;
  if (prettyPrint){
    _userData.prettyPrintTo(userJson);
  }else{
    _userData.printTo(userJson);  
  }
  
  return userJson;
}

void Users::saveUser(char userId[], String firstName){
  JsonObject& data = _userData.createNestedObject(userId);
  data["firstName"]=firstName;
}

void Users::deleteUser(char userId[]){
  _userData.remove(userId);
}
#endif // _USERS_H_

