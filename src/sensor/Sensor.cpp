#include "Sensor.h"

Sensor::Sensor() {
  
}

void Sensor::init(float safeLimit) {
  _safeLimit = safeLimit;
}

float Sensor::getSafeLimit() {
  return _safeLimit;
}

void Sensor::setSafeLimit(float safeLimit) {
  _safeLimit = safeLimit;
}

bool Sensor::aboveSafeLimit() {
  // Read sensor
  return _currentReading > _safeLimit;
}

bool Sensor::belowSafeLimit() {
  // Read sensor
  return _currentReading < _safeLimit;
}

bool Sensor::getInitialization() {
  return _hasInitialized;
}