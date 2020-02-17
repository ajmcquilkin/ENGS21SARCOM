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
#include <ControlRelay.h>

#define PRESS_THRESHOLD 1.00
#define TEMP_THRESHOLD 1.00
#define HUM_THRESHOLD 1.00
#define WIND_THRESHOLD 1.00

#define VOLTAGE_ONE_PIN A0
#define VOLTAGE_TWO_PIN A1
#define VOLTAGE_THREE_PIN A2

#define LED_PIN_R 3
#define LED_PIN_G 5
#define LED_PIN_B 6

#define VOLTAGE_THRESHOLD 2
int voltageOne, voltageTwo, voltageThree, pastVoltageOne, pastVoltageTwo, pastVoltageThree;

void setup() {
  Serial.begin(9600);

  pinMode(VOLTAGE_ONE_PIN, INPUT);
  pinMode(VOLTAGE_TWO_PIN, INPUT);
  pinMode(VOLTAGE_THREE_PIN, INPUT);

  pinMode(LED_PIN_R, OUTPUT);
  pinMode(LED_PIN_G, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);

  ClimateSensor clim = ClimateSensor(4);
  clim.init();

  ControlRelay cr = ControlRelay(6);
  cr.init();

  cr.enablePower();
  delay(2000);
  cr.disablePower();


  Serial.println(clim.getCurrentTemperature());
  Serial.println(clim.getCurrentHumidity());

  pastVoltageOne = analogRead(VOLTAGE_ONE_PIN);
  pastVoltageTwo = analogRead(VOLTAGE_TWO_PIN);
  pastVoltageThree = analogRead(VOLTAGE_THREE_PIN);
}

void loop() {
  voltageOne = analogRead(VOLTAGE_ONE_PIN);
  voltageTwo = analogRead(VOLTAGE_TWO_PIN);
  voltageThree = analogRead(VOLTAGE_THREE_PIN);

  analogWrite(LED_PIN_R, map(voltageOne, 1023, 0, 0, 255));
  analogWrite(LED_PIN_G, map(voltageTwo, 1023, 0, 0, 255));
  analogWrite(LED_PIN_B, map(voltageThree, 1023, 0, 0, 255));

  bool voltageOneDisplay = abs(voltageOne - pastVoltageOne) > VOLTAGE_THRESHOLD;
  bool voltageTwoDisplay = abs(voltageTwo - pastVoltageTwo) > VOLTAGE_THRESHOLD;
  bool voltageThreeDisplay = abs(voltageThree - pastVoltageThree) > VOLTAGE_THRESHOLD;

  if (voltageOneDisplay || voltageTwoDisplay || voltageThreeDisplay) {
    Serial.print("Voltage One: ");
    Serial.print(voltageOne);
    Serial.print("\t");
    Serial.print("Voltage Two: ");
    Serial.print(voltageTwo);
    Serial.print("\t");
    Serial.print("Voltage Three: ");
    Serial.print(voltageThree);
    Serial.println();
  }

  pastVoltageOne = voltageOne;
  pastVoltageTwo = voltageTwo;
  pastVoltageThree = voltageThree;

  delay(1);
}