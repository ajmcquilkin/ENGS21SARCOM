#include <Sensor.h>

class Wind : public Sensor {
  public:
    Wind() : Sensor() {
      
    }

    void init();
};