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
*/

#include <Arduino.h>
#include <ClimateSensor.h>
#include <ControlRelay.h>
#include <ControlLED.h>

#define MIN_TEMP -20
#define MAX_TEMP 80
#define MAX_HUM 80

#define VOLTAGE_ONE_PIN A0
#define VOLTAGE_TWO_PIN A1
#define VOLTAGE_THREE_PIN A2

#define LED_PIN_R 3
#define LED_PIN_G 5
#define LED_PIN_B 6

#define VOLTAGE_THRESHOLD 2
int voltageOne, voltageTwo, voltageThree, pastVoltageOne, pastVoltageTwo, pastVoltageThree;

ClimateSensor clim = ClimateSensor(4);
ControlRelay cr = ControlRelay(7);
ControlLED cl = ControlLED(3, 5, 6);

void setup() {
  Serial.begin(9600);

  pinMode(VOLTAGE_ONE_PIN, INPUT);
  pinMode(VOLTAGE_TWO_PIN, INPUT);
  pinMode(VOLTAGE_THREE_PIN, INPUT);

  pinMode(LED_PIN_R, OUTPUT);
  pinMode(LED_PIN_G, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);

  clim.init();
  cr.init();
  cl.init();

  cr.enablePower();
  delay(2000);

  cr.disablePower();
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

  Serial.println(clim.getCurrentTemperature());
  Serial.println(clim.getCurrentHumidity());

  pastVoltageOne = analogRead(VOLTAGE_ONE_PIN);
  pastVoltageTwo = analogRead(VOLTAGE_TWO_PIN);
  pastVoltageThree = analogRead(VOLTAGE_THREE_PIN);
}

void loop() {
  // voltageOne = analogRead(VOLTAGE_ONE_PIN);
  // voltageTwo = analogRead(VOLTAGE_TWO_PIN);
  // voltageThree = analogRead(VOLTAGE_THREE_PIN);

  // analogWrite(LED_PIN_R, map(voltageOne, 1023, 0, 0, 255));
  // analogWrite(LED_PIN_G, map(voltageTwo, 1023, 0, 0, 255));
  // analogWrite(LED_PIN_B, map(voltageThree, 1023, 0, 0, 255));

  // bool voltageOneDisplay = abs(voltageOne - pastVoltageOne) > VOLTAGE_THRESHOLD;
  // bool voltageTwoDisplay = abs(voltageTwo - pastVoltageTwo) > VOLTAGE_THRESHOLD;
  // bool voltageThreeDisplay = abs(voltageThree - pastVoltageThree) > VOLTAGE_THRESHOLD;

  // if (voltageOneDisplay || voltageTwoDisplay || voltageThreeDisplay) {
  //   Serial.print("Voltage One: ");
  //   Serial.print(voltageOne);
  //   Serial.print("\t");
  //   Serial.print("Voltage Two: ");
  //   Serial.print(voltageTwo);
  //   Serial.print("\t");
  //   Serial.print("Voltage Three: ");
  //   Serial.print(voltageThree);
  //   Serial.println();
  // }

  // pastVoltageOne = voltageOne;
  // pastVoltageTwo = voltageTwo;
  // pastVoltageThree = voltageThree;

  // delay(1);

  checkSystemStatus();
  delay(50);
}

void checkSystemStatus() {
  int temp, humid, press;

  temp = clim.getCurrentTemperature();
  humid = clim.getCurrentHumidity();
  // press = clim.getCurrentPressure();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("\t");
  Serial.print("Humidity: ");
  Serial.print(humid);
  // Serial.print("\t");
  // Serial.print("Voltage Three: ");
  // Serial.print(voltageThree);
  Serial.println();

  if (temp < MIN_TEMP || MAX_TEMP < temp) {
    cl.setLEDState(ControlLED::TEMPERROR);
  } else if (MAX_HUM < humid) {
    cl.setLEDState(ControlLED::HUMIDERROR);
  } else {
    cl.setLEDState(ControlLED::OK);
  }
}