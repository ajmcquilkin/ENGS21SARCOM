/*
   Creating an engine to process input from a number of sources and determine if the repeater is still safe to operate

   Sensors
   - Top:
   - - Wind
   - - Temperature
   - - Humidity
   - Bottom:
   - - Temperature
   - - Humidity
   - - Battery power
*/

#include <Arduino.h>
#include <ClimateSensor.h>

#define PRESS_THRESHOLD 1.00
#define TEMP_THRESHOLD 1.00
#define HUM_THRESHOLD 1.00
#define WIND_THRESHOLD 1.00

#define VOLTAGE_ONE_PIN A0
#define VOLTAGE_TWO_PIN A1

#define VOLTAGE_THRESHOLD 2
int voltageOne, voltageTwo, pastVoltageOne, pastVoltageTwo;

void setup() {
  Serial.begin(9600);

  pinMode(VOLTAGE_ONE_PIN, INPUT);
  pinMode(VOLTAGE_TWO_PIN, INPUT);

  ClimateSensor clim = ClimateSensor();
  clim.init();

  Serial.println(clim.getCurrentTemperature());
  Serial.println(clim.getCurrentHumidity());

  pastVoltageOne = analogRead(VOLTAGE_ONE_PIN);
  pastVoltageTwo = analogRead(VOLTAGE_TWO_PIN);
}

void loop() {
  voltageOne = analogRead(VOLTAGE_ONE_PIN);
  voltageTwo = analogRead(VOLTAGE_TWO_PIN);

  bool voltageOneDisplay = abs(voltageOne - pastVoltageOne) > VOLTAGE_THRESHOLD;
  bool voltageTwoDisplay = abs(voltageTwo - pastVoltageTwo) > VOLTAGE_THRESHOLD;

  if (voltageOneDisplay || voltageTwoDisplay) {
    Serial.print("Voltage One: ");
    Serial.print(voltageOne);
    Serial.print("\t");
    Serial.print("Voltage Two: ");
    Serial.println(voltageTwo);
  }

  pastVoltageOne = voltageOne;
  pastVoltageTwo = voltageTwo;

  delay(50);
}