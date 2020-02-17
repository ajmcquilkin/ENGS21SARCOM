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

#include <Temperature.h>
#include <Humidity.h>
#include <Wind.h>
#include <BatteryController.h>

#include <Arduino.h>

#define TEMP_THRESHOLD 1.00
#define HUM_THRESHOLD 1.00
#define WIND_THRESHOLD 1.00

#define VOLTAGE_ONE_PIN A0
#define VOLTAGE_TWO_PIN A1

#define VOLTAGE_THRESHOLD 2
int voltageOne, voltageTwo, pastVoltageOne, pastVoltageTwo;

void setup() {
  Serial.begin(115200);

  pinMode(VOLTAGE_ONE_PIN, INPUT);
  pinMode(VOLTAGE_TWO_PIN, INPUT);

  Temperature temp = Temperature();
  Humidity hum = Humidity();
  Wind wind = Wind();

  temp.init(1.00);
  hum.init(1.00);
  wind.init(1.00);

  Serial.println(temp.getSafeLimit());
  Serial.println(hum.getSafeLimit());
  Serial.println(wind.getSafeLimit());

  pastVoltageOne = analogRead(VOLTAGE_ONE_PIN);
  pastVoltageTwo = analogRead(VOLTAGE_TWO_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  voltageOne = analogRead(VOLTAGE_ONE_PIN);
  voltageTwo = analogRead(VOLTAGE_TWO_PIN);

  bool voltageOneDisplay = abs(voltageOne - pastVoltageOne) > VOLTAGE_THRESHOLD;
  bool voltageTwoDisplay = abs(voltageTwo - pastVoltageTwo) > VOLTAGE_THRESHOLD;

  if (voltageOneDisplay) {
    Serial.print("Voltage One: ");
    Serial.print(voltageOne);
  } else if (voltageOneDisplay && voltageTwoDisplay) {
    Serial.print("\t");
  } else if (voltageTwoDisplay) {
    Serial.print("Voltage Two: ");
    Serial.print(voltageTwo);
  }

  if (voltageOneDisplay || voltageTwoDisplay) {
    Serial.println();
  }

  pastVoltageOne = voltageOne;
  pastVoltageTwo = voltageTwo;

  delay(50);
}
