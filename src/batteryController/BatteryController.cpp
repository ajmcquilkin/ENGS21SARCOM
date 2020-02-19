#include <Arduino.h>
#include "BatteryController.h"
using namespace std;

void BatteryController::init() {
  pinMode(_pin, INPUT);
}

float BatteryController::getBatteryVoltage() {
  _checkBatteryVoltage();
  return _voltage;
}

bool BatteryController::batteryWithinRange() {
  return _checkBatteryVoltage();
}

float BatteryController::getMinBatteryVoltage() {
  return _minSafeVoltage;
}

float BatteryController::getMaxBatteryVoltage() {
  return _maxSafeVoltage;
}

bool BatteryController::_checkBatteryVoltage() {
  _readBatteryVoltage();
  if (_voltage < _minSafeVoltage || _maxSafeVoltage < _voltage) {
    return false;
  } return true;
}

float BatteryController::_readBatteryVoltage() {
  _voltage = analogRead(_pin) * (_voltageReference / _referenceResolution);
}