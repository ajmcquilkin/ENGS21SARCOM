#include <Arduino.h>
#include <ControlRelay.h>
using namespace std;

ControlRelay::ControlRelay(int pin, bool activeLow) {
  _activeLow = activeLow;
  _powerEnabled = false;
  _pin = pin;
}

void ControlRelay::init() {
  pinMode(_pin, OUTPUT);
}

int ControlRelay::getPin() {
  return _pin;
}

void ControlRelay::setPin(int pin) {
  _pin = pin;
  init();
}

bool ControlRelay::powerEnabled() {
  return _powerEnabled;
}

bool ControlRelay::powerDisabled() {
  return !_powerEnabled;
}

void ControlRelay::enablePower() {
  digitalWrite(_pin, !_activeLow);
}

void ControlRelay::disablePower() {
  digitalWrite(_pin, _activeLow);
}

void ControlRelay::setPowerState(bool enablePower) {
  digitalWrite(_pin, enablePower);
}