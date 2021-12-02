#include <HTTPClient.h>
#include <WiFi.h>
#include "ArduinoJson.h"
#include "Servo.h"
HTTPClient client; //get
HTTPClient http;   //post request
HTTPClient http2;  //post to notification container

//sensor object

#define MAIN_SSID ""
#define MAIN_PASS ""

#define CSE_IP ""
#define CSE_PORT 8080
#define HTTPS false
#define OM2M_ORGIN "admin:admin"
#define OM2M_MN "/~/in-cse/in-name/"
#define OM2M_AE "Permission_Details"  //for GET request
#define OM2M_DATA_CONT "Notification" //for GET requenst
#define SENSOR_AE "Sensor"            //post
#define DATA_CONT "Data"              //post

String Sensor_Data;
String Status;
String Status2;

int door = 0;
const int nearLED = 22;
const int dooropens = 23;
const int buzzer = 19;
const int trig = 5;
const int echo = 18;
static const int servoPin = 4;

Servo servo1;

#define SOUND 0.0343

long duration;
float distanceCm;

String fetchData(String payload)
{
    //  Serial.println(payload);
    Serial.println("_________________________________________");
    StaticJsonDocument<500> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err)
    {
        Serial.print("Error: ");
        Serial.println(err.c_str());
        return "Error";
    }
    JsonObject m2m_cin = doc["m2m:cin"];
    String flag = m2m_cin["con"]; // "X"
    Serial.print("Content: ");
    Serial.println(flag);
    return flag;
}

void setup()
{
    Serial.begin(9600);
    WiFi.begin(MAIN_SSID, MAIN_PASS);
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(nearLED, OUTPUT);
    pinMode(dooropens, OUTPUT);
    pinMode(buzzer, OUTPUT);
    servo1.attach(servoPin);
}

int i = 0;

void loop()
{

    String Server = "http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;
    client.begin(Server + String() + OM2M_AE + "/" + OM2M_DATA_CONT + "/la"); //"/la" for latest, "/ol" for oldest, "?rcn=4" for all data // get x, m, n
    http.begin(Server + String() + SENSOR_AE + "/" + DATA_CONT + "/");        //post ultra-sonic sensor
    http2.begin(Server + String() + OM2M_AE + "/" + OM2M_DATA_CONT + "/");    // to post x

    http.addHeader("X-M2M-Origin", OM2M_ORGIN);
    http.addHeader("Content-Type", "application/json;ty=4");
    http.addHeader("Content-Length", "100");

    http2.addHeader("X-M2M-Origin", OM2M_ORGIN);
    http2.addHeader("Content-Type", "application/json;ty=4");
    http2.addHeader("Content-Length", "100");

    client.addHeader("X-M2M-Origin", "guest:guest");
    client.addHeader("Accept", "application/json");

    int GET_code = client.GET();
    Serial.println(GET_code);
    String payload = client.getString();
    String flag = fetchData(payload);

    //Sensor DATA----------------
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    duration = pulseIn(echo, HIGH);
    distanceCm = duration * SOUND / 2;

    Sensor_Data = String(distanceCm);

    String req_data = String() + "{\"m2m:cin\": {" + "\"con\": \"" + Sensor_Data + "\"," + "\"rn\": \"" + "cin_" + String(i++) + "\"," + "\"cnf\": \"text\"" + "}}";
    Serial.println(distanceCm);
    int code = http.POST(req_data);
    Serial.println(code);
    http.end();
//    Serial.println(distanceCm);/
    

    if (distanceCm < 100)
    {
        digitalWrite(nearLED, HIGH);
//        server.begin();
        if (flag == "M")
        {
            door++;
            digitalWrite(buzzer, LOW);
            digitalWrite(dooropens, HIGH);
            for (int posDegrees = 0; posDegrees <= 90; posDegrees++)
            {
                servo1.write(posDegrees);
                delay(20);
            }
            delay(5000);
            for (int posDegrees = 90; posDegrees >= 0; posDegrees--)
            {
                servo1.write(posDegrees);
                delay(20);
            }
            digitalWrite(dooropens, LOW);

            Status = "X";

            String req_data2 = String() + "{\"m2m:cin\": {" + "\"con\": \"" + Status + "\"," + "\"rn\": \"" + "cin_" + String(i++) + "\"," + "\"cnf\": \"text\"" + "}}";
            int code2 = http2.POST(req_data2);
            Serial.println(code2);
        }
        else if (flag == "N")
        {
            digitalWrite(buzzer, HIGH);
            delay(5000);

            Status2 = "X";

            String req_data3 = String() + "{\"m2m:cin\": {"

                               + "\"con\": \"" + Status2 + "\","

                               + "\"rn\": \"" + "cin_" + String(i++) + "\","

                               + "\"cnf\": \"text\""

                               + "}}";
            int code3 = http2.POST(req_data3);
            Serial.println(code3);
        }
    }
    else
    {
        digitalWrite(buzzer, LOW);
        digitalWrite(nearLED, LOW);
    }
    //--------------------------

    http2.end();
    client.end();
    delay(1000);
}
