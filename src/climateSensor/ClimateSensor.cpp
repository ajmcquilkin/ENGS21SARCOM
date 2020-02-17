#include <DHT.h>
#include <ClimateSensor.h>
using namespace std;

void ClimateSensor::init() {
  _dht.begin();
}

void ClimateSensor::_readSensor() {
  _temperature = _dht.readTemperature();
  _humidity = _dht.readHumidity();
  // _pressure = _dht.readPressure(); // Not yet supported
}
