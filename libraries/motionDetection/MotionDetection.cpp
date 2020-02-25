#include <Arduino.h>
#include <MotionDetection.h>
using namespace std;

void MotionDetection::init() {
  _imu.begin();
  _imu.setRange(ADXL343_RANGE_16_G);
}

bool MotionDetection::hasFallen(){
  _readSensor();

  if (_zAccel < _zAccelMinThreshold) {
    return true;
  } return false;
}

void MotionDetection::setMinAccelThreshold(float minAccelThreshold) {
  _zAccelMinThreshold = minAccelThreshold;
}

float MotionDetection::getZAccel() {
  _readSensor();
  return _zAccel;
}

void MotionDetection::_readSensor() {
  sensors_event_t event;
  _imu.getEvent(&event);

  _xAccel = event.acceleration.x;
  _yAccel = event.acceleration.y;
  _zAccel = event.acceleration.z;

  // _xAccel = _imu.getX();
  // _yAccel = _imu.getY();
  // _zAccel = _imu.getZ();

  // _xGyro = event.gyro.x;
  // _yGyro = event.gyro.y;
  // _zGyro = event.gyro.z;
}