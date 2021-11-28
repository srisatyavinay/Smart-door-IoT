#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ThingSpeak.h>
#include <Servo.h>

const char* password = "xxxx"; //Change this
const char* ssid = "xxxx"; //Change this

AsyncWebServer server(80);
WiFiClient  client;
unsigned long myChannelNumber = 1; //Change this
const char * myWriteAPIKey = "xxxx"; //Change this

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

const char* htmlname = "name";
const char* htmlpass = "password";

String inputName;
String inputPassword;
String pass = "abcd";
String nullstring = "";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>IoT-project</title>
    <style>
        div {
            margin: auto;
            width: 60%;
            border: 3px solid #73AD21;
            padding: 10px;
            background-color: rgb(255, 253, 164);
        }
        body {
            background-color: rgb(255, 220, 154);
        }
    </style>
</head>
<body>
    <form action="/get">
        <table>
            <tbody>
                <tr>
                    <td>
                        <label for="personName">Name:&nbsp;&nbsp;</label>
                    </td>
                    <td>
                        <input type="text" id="personname" name="name" id="personName" required placeholder="Enter your name">
                    </td>
                </tr>
                <tr>
                    <td>
                        <label for="theirpassword">Password:</label>
                    </td>
                    <td>
                        <input type="password" name="password" id="theirpassword" required placeholder="Enter Password">
                    </td>
                </tr>
                <tr>
                    <td><input type="submit" value="Submit"></td>
                </tr>
            </tbody>
        </table>
    </form>
</body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(nearLED, OUTPUT);
  pinMode(dooropens, OUTPUT);
  pinMode(buzzer, OUTPUT);
  servo1.attach(servoPin);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    inputName = request->getParam(htmlname)->value();
    inputPassword = request->getParam(htmlpass)->value();
    Serial.println(inputName);
    Serial.println(inputPassword);
    request->send(200, "text/html", "Your name " + inputName + " is sent to ESP32" + "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
  ThingSpeak.begin(client);
}

void loop() {
  int x = ThingSpeak.writeField(myChannelNumber, 2, door, myWriteAPIKey);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distanceCm = duration * SOUND/2;
//  Serial.print("Distance in cm: ");
//  Serial.println(distanceCm);
//  Serial.print("1. ");
//  Serial.println(inputPassword);
//  Serial.println();
  if(distanceCm < 100)
  {
    digitalWrite(nearLED, HIGH);
//    Serial.print("2. ");
//    Serial.println(inputPassword);
//    Serial.println();
    server.begin();
//    Serial.print("3. ");
//    Serial.println(inputPassword);
//    Serial.println();
    if(inputPassword == pass)
    {
      door++;
      digitalWrite(buzzer, LOW);
      digitalWrite(dooropens, HIGH);
      for(int posDegrees = 0; posDegrees <= 90; posDegrees++) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
      }
      delay(5000);
      for(int posDegrees = 90; posDegrees >= 0; posDegrees--) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
      }
      digitalWrite(dooropens, LOW);
      inputName = "";
      inputPassword = "";
    }
    else if(inputPassword != nullstring && inputPassword != pass)
    {
      digitalWrite(buzzer, HIGH);
      delay(5000);
      inputName = "";
      inputPassword = "";
    }
  }
  else
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(nearLED, LOW);
    server.end();
  }
}
