#ifndef SENSOR_H
#define SENSOR_H

class Sensor {
  public:
    Sensor();

    void init(float safeLimit);

    float getSafeLimit();
    void setSafeLimit(float safeLimit);
    bool aboveSafeLimit();
    bool belowSafeLimit();

    bool getInitialization();

  private:
    float _safeLimit;
    float _currentReading;
    bool _hasInitialized = false;
};

#endif