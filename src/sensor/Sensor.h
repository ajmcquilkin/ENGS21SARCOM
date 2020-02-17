#ifndef SENSOR_H
#define SENSOR_H

class Sensor {
  public:
    Sensor();

    // void init(float safeLimit);

    float getSafeLimit();
    void setSafeLimit(float safeLimit);
    bool aboveSafeLimit();
    bool belowSafeLimit();

    bool getInitialization();
    float getCurrentReading();

  protected:
    float _safeLimit;
    float _currentReading;
    bool _hasInitialized = false;

    void _readSensor();
};

#endif