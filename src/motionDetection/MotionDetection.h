#include <Adafruit_ADXL343.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class MotionDetection {
  public:
    MotionDetection() : _imu(12345) {

    }
      
    void init();
    bool hasFallen();

    float getZAccel();

  private:
    float _xAccel, _yAccel, _zAccel, _xGyro, _yGyro, _zGyro;
    void _readSensor();

    Adafruit_ADXL343 _imu;
};