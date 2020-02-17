#include <DHT.h>
#include <ClimateSensor.h>
using namespace std;

void ClimateSensor::init() {
  _dht.begin();
}

void ClimateSensor::_readSensor() {
  _currentTemperature = _dht.readTemperature();
  _currentHumidity = _dht.readHumidity();
  // _currentPressure = _dht.readPressure(); // Not yet supported
}

float ClimateSensor::getCurrentTemperature() {
  _readSensor();
  return _currentTemperature;
}

float ClimateSensor::getCurrentHumidity() {
  _readSensor();
  return _currentHumidity;
}

// float ClimateSensor::getCurrentPressure() {
//   _readSensor();
//   return _currentPressure;
// }
