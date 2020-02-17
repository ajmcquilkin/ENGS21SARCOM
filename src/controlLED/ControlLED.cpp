#include <Arduino.h>
#include <ControlLED.h>
using namespace std;

ControlLED::ControlLED(int rPin, int gPin, int bPin) {
  _rPin = rPin;
  _gPin = gPin;
  _bPin = bPin;
}

void ControlLED::init() {
  pinMode(_rPin, OUTPUT);
  pinMode(_gPin, OUTPUT);
  pinMode(_bPin, OUTPUT);
}

void ControlLED::setRGB(int r, int g, int b) {
  analogWrite(_rPin, r);
  analogWrite(_gPin, g);
  analogWrite(_bPin, b);
}

// void fadeToRGB(int r, int g, int b, int msDuration) {
  
// }