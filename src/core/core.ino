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

// #include <ClimateSensor.h>
// #include <Wind.h>
// #include <BatteryController.h>

// #include <Arduino.h>

// #define PRESS_THRESHOLD 1.00
// #define TEMP_THRESHOLD 1.00
// #define HUM_THRESHOLD 1.00
// #define WIND_THRESHOLD 1.00

// #define VOLTAGE_ONE_PIN A0
// #define VOLTAGE_TWO_PIN A1

// #define VOLTAGE_THRESHOLD 2
// int voltageOne, voltageTwo, pastVoltageOne, pastVoltageTwo;

// void setup() {
//   Serial.begin(115200);

//   pinMode(VOLTAGE_ONE_PIN, INPUT);
//   pinMode(VOLTAGE_TWO_PIN, INPUT);

//   // ClimateSensor clim = ClimateSensor();
//   // Wind wind = Wind();

//   // clim.init();
//   // wind.init();

//   // Serial.println(clim.getSafeLimit());
//   // Serial.println(wind.getSafeLimit());

//   pastVoltageOne = analogRead(VOLTAGE_ONE_PIN);
//   pastVoltageTwo = analogRead(VOLTAGE_TWO_PIN);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   voltageOne = analogRead(VOLTAGE_ONE_PIN);
//   voltageTwo = analogRead(VOLTAGE_TWO_PIN);

//   bool voltageOneDisplay = abs(voltageOne - pastVoltageOne) > VOLTAGE_THRESHOLD;
//   bool voltageTwoDisplay = abs(voltageTwo - pastVoltageTwo) > VOLTAGE_THRESHOLD;

//   if (voltageOneDisplay) {
//     Serial.print("Voltage One: ");
//     Serial.print(voltageOne);
//   } else if (voltageOneDisplay && voltageTwoDisplay) {
//     Serial.print("\t");
//   } else if (voltageTwoDisplay) {
//     Serial.print("Voltage Two: ");
//     Serial.print(voltageTwo);
//   }

//   if (voltageOneDisplay || voltageTwoDisplay) {
//     Serial.println();
//   }

//   pastVoltageOne = voltageOne;
//   pastVoltageTwo = voltageTwo;

//   delay(50);
// }

#include <Arduino.h>
#include "DHT.h"

DHT dht(4, DHT11);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
