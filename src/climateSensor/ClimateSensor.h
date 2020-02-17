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
  
  private:
    DHT _dht;
    
    float _temperature;
    float _pressure;
    float _humidity;
};