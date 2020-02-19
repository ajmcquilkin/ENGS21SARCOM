#include <Arduino.h>
#include <Sensor.h>

class BatteryController : public Sensor {
  public:
    enum BatteryType { LiPo, NiMH };
    BatteryController(int pin, BatteryType b, float voltageReference=5.00, int referenceResolution=1024) : Sensor() {
      _pin = pin;
      _voltageReference = voltageReference;
      _referenceResolution = referenceResolution;
      _batteryType = b;

      switch (_batteryType) {
        case BatteryType::LiPo:
          _minSafeVoltage = 3.7;
          _maxSafeVoltage = 4.2;
          break;

        case BatteryType::NiMH:
          _minSafeVoltage = 1.0;
          _maxSafeVoltage = 1.5;
          break;

        default:
          _minSafeVoltage = -1.0;
          _maxSafeVoltage = -1.0;
          break;
      }
    }

    void init();
    float getBatteryVoltage();
    bool batteryWithinRange();

    float getMinBatteryVoltage();
    float getMaxBatteryVoltage();

  private:
    int _pin;
    BatteryType _batteryType;

    float _voltage;
    float _voltageReference;
    int _referenceResolution;

    float _minSafeVoltage;
    float _maxSafeVoltage;

    bool _checkBatteryVoltage();
    float _readBatteryVoltage();
};