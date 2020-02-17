#include <Arduino.h>
#include <ControlLED.h>
using namespace std;

ControlLED::ControlLED(int rPin, int gPin, int bPin, bool commonAnode=true, int resolution=255) {
  _rPin = rPin;
  _gPin = gPin;
  _bPin = bPin;

  _commonAnode = commonAnode;
  _resolution = resolution;
}

void ControlLED::init() {
  pinMode(_rPin, OUTPUT);
  pinMode(_gPin, OUTPUT);
  pinMode(_bPin, OUTPUT);
}

void ControlLED::setRGB(int r, int g, int b) {
  _rValue = r;
  _gValue = g;
  _bValue = b;

  analogWrite(_rPin, _commonAnode ? _resolution - _rValue : _rValue);
  analogWrite(_gPin, _commonAnode ? _resolution - _gValue : _gValue);
  analogWrite(_bPin, _commonAnode ? _resolution - _bValue : _bValue);
}

// void fadeToRGB(int r, int g, int b, int msDuration) {
  
// }