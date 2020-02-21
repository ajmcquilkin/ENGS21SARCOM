/*
  Creating an engine to process input from a number of sources and determine if the repeater is still safe to operate

  Sensors
  - Top:
  - - Wind
  - - Temperature
  - - Humidity
  - - Accelerometer
  - Bottom:
  - - Temperature
  - - Humidity
  - - Battery power

  For transmitting sensor data from top of mast:
  - "<" + SENSOR_CODE_1 + ":" SENSOR_DATA_1 + "," + ... (repeat same format) ... + ">"
  - On initialization of RS485 communication, MASTER will transmit SensorCode enum to slave
*/

#include <Arduino.h>
#include <ClimateSensor.h>
#include <ControlRelay.h>
#include <ControlLED.h>
#include <BatteryController.h>
#include <MotionDetection.h>

#define MIN_TEMP -20
#define MAX_TEMP 80
#define MAX_HUM 80

// Codes for each sensor, should maintain consistency with all boards in system
// TODO: Make a method for transmitting this data to all SLAVE boards
enum SensorCode {
  ACCEL_SENSOR_X = 0b00000000,
  ACCEL_SENSOR_Y = 0b00000001,
  ACCEL_SENSOR_Z = 0b00000010,

  GYRO_SENSOR_X = 0b00000011,
  GYRO_SENSOR_Y = 0b00000100,
  GYRO_SENSOR_Z = 0b00000101,

  HUMIDITY_SENSOR = 0b00000110,
  PRESSURE_SENSOR = 0b00000111,
  TEMPERATURE_SENSOR = 0b00001000,
};

ClimateSensor clim = ClimateSensor(4, true);
ControlRelay cr = ControlRelay(7);
ControlLED cl = ControlLED(3, 5, 6);

BatteryController b1 = BatteryController(A0, BatteryController::LiPo);
BatteryController b2 = BatteryController(A1, BatteryController::NiMH);

MotionDetection imu = MotionDetection();

void setup() {
  Serial.begin(9600);

  clim.init();
  cr.init();
  cl.init();
  imu.init();

  cr.enablePower();
  // delay(2000);
  // systemCycleTest();

  Serial.println(clim.getCurrentTemperature());
  Serial.println(clim.getCurrentHumidity());
}

void loop() {
  checkSystemStatus();
  delay(100);
}

void checkSystemStatus() {
  int temp, humid, press;

  temp = clim.getCurrentTemperature();
  humid = clim.getCurrentHumidity();
  // press = clim.getCurrentPressure();
  float b1Volt = b1.getBatteryVoltage();
  float b2Volt = b2.getBatteryVoltage();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("\t");
  Serial.print("Humidity: ");
  Serial.print(humid);
  Serial.print("\t");
  // Serial.print("Pressure: ");
  // Serial.print(press);
  Serial.print("Battery 1 (LiPo): ");
  Serial.print(b1Volt);
  Serial.print("\t");
  Serial.print("Battery 2 (NiMH): ");
  Serial.print(b2Volt);
  Serial.print("\t");
  Serial.print("Z-Accel: ");
  Serial.print(imu.getZAccel());
  Serial.println();

  // Note: This will determine the relative importance of each error
  if (MAX_HUM < humid) {
    cl.setLEDState(ControlLED::HUMIDERROR);
  } else if (temp < MIN_TEMP || MAX_TEMP < temp) {
    cl.setLEDState(ControlLED::TEMPERROR);
  } else if (!b1.batteryWithinRange() || !b2.batteryWithinRange()) {
    cl.setLEDState(ControlLED::POWERERROR);
  } else if (imu.hasFallen()) {
    cl.setLEDState(ControlLED::WARNING);
  } else {
    cl.setLEDState(ControlLED::OK);
  }
  
  if (cl.getLedState() != ControlLED::OK) {
    cr.disablePower();
  } else {
    cr.enablePower();
  }
}

void systemCycleTest() {
  cr.disablePower();
  delay(2000);
  
  cr.enablePower();
  delay(2000);

  cl.setLEDState(ControlLED::UNDEF);
  Serial.println("UNDEF");
  delay(3000);

  cl.setLEDState(ControlLED::SYSTEMERROR);
  Serial.println("SYSTEMERROR");
  delay(3000);

  cl.setLEDState(ControlLED::TEMPERROR);
  Serial.println("TEMPERROR");
  delay(3000);

  cl.setLEDState(ControlLED::HUMIDERROR);
  Serial.println("HUMIDERROR");
  delay(3000);

  cl.setLEDState(ControlLED::POWERERROR);
  Serial.println("POWERERROR");
  delay(3000);

  cl.setLEDState(ControlLED::WARNING);
  Serial.println("WARNING");
  delay(3000);

  cl.setLEDState(ControlLED::OK);
  Serial.println("OK");
  delay(3000);
}