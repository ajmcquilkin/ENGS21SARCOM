#include <Arduino.h>
#include "SoftwareSerial.h"

#include <ClimateSensor.h>
#include <MotionDetection.h>

#define MIN_TEMP -20
#define MAX_TEMP 80
#define MAX_HUM 80

#define ssEnable 8
#define ssRX 9
#define ssTX 10

unsigned long nonVitalPreviousMillis = 0;
const long nonVitalPollingInterval = 1000;

unsigned long vitalPreviousMillis = 0;
const long vitalPollingInterval = 200;

// TODO: Load this from MASTER microcontroller
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
MotionDetection imu = MotionDetection();

SoftwareSerial sensorDownlink = SoftwareSerial(ssRX, ssTX);

void setup() {
  Serial.begin(9600);
  sensorDownlink.begin(9600);

  imu.init();
  clim.init();

  pinMode(ssEnable, OUTPUT); // MASTER/SLAVE selector
  delay(10);
  digitalWrite(ssEnable, HIGH); // Master needs enable HIGH
}

/**
 * TODO: Make the loop differentiate b/w urgent and non-urgent sensor data and poll sensors and transmit data accordingly
*/
void loop() {
  unsigned long currentMillis = millis();

  // Non-blocking delay for vital sensors
  if (currentMillis - vitalPreviousMillis > vitalPollingInterval) {
    vitalPreviousMillis = currentMillis;

    // Serial.println((int)imu.getZAccel());
    // Serial.println(imu.hasFallen());

    // Check if the imu has fallen every vitalPollingInterval
    if (imu.hasFallen() == true) {
      Serial.println("Vital - fall detected");

      // ----------- Serial writing ----------- //
      
      Serial.write('<');
      Serial.print(SensorCode::ACCEL_SENSOR_Z);
      Serial.write(':');
      Serial.print((int)imu.getZAccel());
      Serial.write('>');

      // ----------- sensorDownlink writing ----------- //

      sensorDownlink.write('<');
      sensorDownlink.print(SensorCode::ACCEL_SENSOR_Z);
      sensorDownlink.write(':');
      sensorDownlink.print((int)imu.getZAccel());
      sensorDownlink.write('>');
    } else {
      // Serial.println("Vital - no fall detected");
    }
  }

  // Non-blocking delay for non-vital sensors
  if (currentMillis - nonVitalPreviousMillis > nonVitalPollingInterval) {
    nonVitalPreviousMillis = currentMillis;

    // ----------- Serial writing ----------- //

    Serial.println("Nonvital - polling");
    // Serial.println((int)imu.getZAccel());
    // Serial.println(imu.hasFallen());

    Serial.write('<');

    Serial.print(SensorCode::HUMIDITY_SENSOR);
    Serial.write(':');
    Serial.print((int)clim.getCurrentHumidity());
    Serial.write(',');

    Serial.print(SensorCode::TEMPERATURE_SENSOR);
    Serial.write(':');
    Serial.print((int)clim.getCurrentTemperature());
    Serial.write(',');
    
    Serial.print(SensorCode::ACCEL_SENSOR_Z);
    Serial.write(':');
    Serial.print((int)imu.getZAccel());
    // Serial.write(',');
    
    Serial.write('>');
    Serial.println();

    // ----------- sensorDownlink writing ----------- //

    sensorDownlink.write('<');

    sensorDownlink.print(SensorCode::HUMIDITY_SENSOR);
    sensorDownlink.write(':');
    sensorDownlink.print((int)clim.getCurrentHumidity());
    sensorDownlink.write(',');

    sensorDownlink.print(SensorCode::TEMPERATURE_SENSOR);
    sensorDownlink.write(':');
    sensorDownlink.print((int)clim.getCurrentTemperature());
    sensorDownlink.write(',');
    
    sensorDownlink.print(SensorCode::ACCEL_SENSOR_Z);
    sensorDownlink.write(':');
    sensorDownlink.print((int)imu.getZAccel());
    // sensorDownlink.write(',');
    
    sensorDownlink.write('>');
    sensorDownlink.println();
  }

  // checkSystemStatus();
}

// TODO: Make these functions into exteral libraries
void checkSystemStatus() {
  // int temp, humid, press;

  // temp = clim.getCurrentTemperature();
  // humid = clim.getCurrentHumidity();
  // press = clim.getCurrentPressure();
  // float b1Volt = b1.getBatteryVoltage();
  // float b2Volt = b2.getBatteryVoltage();

  // Serial.print("Temperature: ");
  // Serial.print(temp);
  // Serial.print("\t");
  // Serial.print("Humidity: ");
  // Serial.print(humid);
  // Serial.print("\t");
  // Serial.print("Pressure: ");
  // Serial.print(press);
  // Serial.print("Battery 1 (LiPo): ");
  // Serial.print(b1Volt);
  // Serial.print("\t");
  // Serial.print("Battery 2 (NiMH): ");
  // Serial.print(b2Volt);
  // Serial.print("\t");
  // Serial.print("Z-Accel: ");
  // Serial.print(imu.getZAccel());
  // Serial.println();

  // Note: This will determine the relative importance of each error
  // if (MAX_HUM < humid) {
  //   cl.setLEDState(ControlLED::HUMIDERROR);
  // } else if (temp < MIN_TEMP || MAX_TEMP < temp) {
  //   cl.setLEDState(ControlLED::TEMPERROR);
  // } else if (!b1.batteryWithinRange() || !b2.batteryWithinRange()) {
  //   cl.setLEDState(ControlLED::POWERERROR);
  // } else if (imu.hasFallen()) {
  //   cl.setLEDState(ControlLED::WARNING);
  // } else {
  //   cl.setLEDState(ControlLED::OK);
  // }
  
  // if (cl.getLedState() != ControlLED::OK) {
  //   cr.disablePower();
  // } else {
  //   cr.enablePower();
  // }
}

void systemCycleTest() {
  // cr.disablePower();
  // delay(2000);
  
  // cr.enablePower();
  // delay(2000);

  // cl.setLEDState(ControlLED::UNDEF);
  // Serial.println("UNDEF");
  // delay(3000);

  // cl.setLEDState(ControlLED::SYSTEMERROR);
  // Serial.println("SYSTEMERROR");
  // delay(3000);

  // cl.setLEDState(ControlLED::TEMPERROR);
  // Serial.println("TEMPERROR");
  // delay(3000);

  // cl.setLEDState(ControlLED::HUMIDERROR);
  // Serial.println("HUMIDERROR");
  // delay(3000);

  // cl.setLEDState(ControlLED::POWERERROR);
  // Serial.println("POWERERROR");
  // delay(3000);

  // cl.setLEDState(ControlLED::WARNING);
  // Serial.println("WARNING");
  // delay(3000);

  // cl.setLEDState(ControlLED::OK);
  // Serial.println("OK");
  // delay(3000);
}