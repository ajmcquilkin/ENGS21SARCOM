#include <DHT.h>
#include <Sensor.h>

/**
 *  Refactor to remove safeLimit 
 */

class ClimateSensor : public Sensor {
  public:
    ClimateSensor(int pin=4) :
      Sensor(), _dht(pin, DHT11) {

    }

    void init();
    void _readSensor();

    float getCurrentTemperature();
    float getCurrentHumidity();
    // float getCurrentPressure();
  
  private:
    DHT _dht;
    
    float _currentTemperature;
    float _currentHumidity;
    // float _currentPressure;
};