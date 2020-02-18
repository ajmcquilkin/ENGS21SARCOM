#include <DHT.h>
#include <Sensor.h>

/**
 *  Refactor to remove safeLimit 
 */

class ClimateSensor : public Sensor {
  public:
    ClimateSensor(int pin, bool fahrenheit=false) :
      Sensor(), _dht(pin, DHT11) {
        _fahrenheit = fahrenheit;
    }

    void init();
    void _readSensor();

    float getCurrentTemperature();
    float getCurrentHumidity();
    // float getCurrentPressure();
  
  private:
    DHT _dht;
    
    bool _fahrenheit;

    float _currentTemperature;
    float _currentHumidity;
    // float _currentPressure;
};