#include "Sensor.h"

Sensor::Sensor() {

}

// void Sensor::init(float safeLimit) {
//   _safeLimit = safeLimit;
//   _readSensor();
//   _hasInitialized = true;
// }

float Sensor::getSafeLimit() {
  return _safeLimit;
}

void Sensor::setSafeLimit(float safeLimit) {
  _safeLimit = safeLimit;
}

bool Sensor::aboveSafeLimit() {
  _readSensor();
  return _currentReading > _safeLimit;
}

bool Sensor::belowSafeLimit() {
  _readSensor();
  return _currentReading < _safeLimit;
}

bool Sensor::getInitialization() {
  return _hasInitialized;
}

float Sensor::getCurrentReading() {
  _readSensor();
  return _currentReading;
}

void Sensor::_readSensor() {
  // Read sensor
}