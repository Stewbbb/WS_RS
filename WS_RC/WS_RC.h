#include <WiFi.h>
//#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
//#include <iostream>
//#include <sstream>
#include "ESP32MX1508.h"
#include "web.h"


class Servo{
  const int frequency = 50;
  const int resolution = 14;
  int channel;
  int minPulse;
  int maxPulse;
  int pin;

public:
  Servo(int _pin, int _minPulseMicros, int _maxPulseMicros, int _channel){
    int periodus = 1000000/50; //1 sec = 1 000 000 micros
    minPulse = float(1<<resolution)*(float(_minPulseMicros)/float(periodus));
    maxPulse = float(1<<resolution)*(float(_maxPulseMicros)/float(periodus));
    pin = _pin;
    channel = _channel;
    ledcSetup(channel, frequency, resolution);
    ledcAttachPin(27, channel);
  }

  Servo(int _pin, int _channel){
    int periodus = 1000000/50; //1 sec = 1 000 000 micros
    int minPulse = float(1<<resolution)*(float(1000)/float(periodus));
    int maxPulse = float(1<<resolution)*(float(2000)/float(periodus));
    pin = _pin;
    channel = _channel;
    ledcSetup(channel, frequency, resolution);
    ledcAttachPin(27, channel);
  }

  void writeAngle(int _angle){
    int angleDuty = map(_angle, 0,180, minPulse, maxPulse); //The map function scales the input variable from the first range to the second
    ledcWrite(channel, angleDuty);
  }
};

AsyncWebServer server(80);
AsyncWebSocket wsCarInput("/CarInput");

MX1508 motorB(25,26, 3, 4);
MX1508 motorA(32, 33, 1, 2);
Servo myServo(27, 500, 2500, 0);
int left;
int right;

// ===========================
// Enter your WiFi credentials
// ===========================
const char* host = "TestHost";
const char* password = "TestPassword";
const int channel = 5;
