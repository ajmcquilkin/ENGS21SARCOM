#include <string.h>
#include <stdio.h>
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

  analogWrite(_rPin, _commonAnode ? 255 : 0);
  analogWrite(_gPin, _commonAnode ? 255 : 0);
  analogWrite(_bPin, _commonAnode ? 255 : 0);
}

void ControlLED::setRGB(int r, int g, int b) {
  _rValue = r;
  _gValue = g;
  _bValue = b;

  analogWrite(_rPin, _commonAnode ? _resolution - _rValue : _rValue);
  analogWrite(_gPin, _commonAnode ? _resolution - _gValue : _gValue);
  analogWrite(_bPin, _commonAnode ? _resolution - _bValue : _bValue);
}

void ControlLED::setLEDState(LEDState s) {
  switch (s) {
    // No loaded state
    case UNDEF:
      setRGB(0, 0, 0); // LED off
      break;

    // Fall, other general failure
    case SYSTEMERROR:
      setRGB(255, 62, 0); // Orange
      break;

    // Temperature too low or high
    case TEMPERROR:
      setRGB(255, 0, 255); // Purple
      break;

    // Humidity too high
    case HUMIDERROR:
      setRGB(0, 255, 233); // Blue
      break;
    
    case POWERERROR:
      setRGB(255, 0, 0); // Red
      break;
    
    case WARNING:
      setRGB(255, 97, 0); // Yellow
      break;
    
    // System normal
    case OK: // Green
      setRGB(0, 255, 0);
      break;
    
    // Fallback
    default: // LED off
      setRGB(0, 0, 0);
      break;
  }
}

// void fadeToRGB(int r, int g, int b, int msDuration) {
  
// }