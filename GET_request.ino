#include<HTTPClient.h>
#include <WiFi.h>
#include "ArduinoJson.h"
// header file
HTTPClient http;


//sensor object 

#define MAIN_SSID "JioFiber-887587"
#define MAIN_PASS "idontknow"

#define CSE_IP      "192.168.29.142"
#define CSE_PORT    8080
#define HTTPS     false
#define OM2M_ORGIN    "admin:admin"
#define OM2M_MN     "/~/in-cse/in-name/"
#define OM2M_AE     "Permission_Details"
#define OM2M_DATA_CONT  "Notification"

void setup() {
Serial.begin(115200);
WiFi.begin(MAIN_SSID,MAIN_PASS);
}

void loop() {
 
String server="http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_MN;

http.begin(server + String() +OM2M_AE + "/" + OM2M_DATA_CONT + "/la"); //"/la" for latest, "/ol" for oldest, "?rcn=4" for all data 

http.addHeader("X-M2M-Origin", "guest:guest");
http.addHeader("Accept", "application/json");

int code = http.GET();
Serial.println(code);
String payload = http.getString();
Serial.println(payload);
http.end();
delay(1000);
}
