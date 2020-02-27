/*
  Creating an engine to process input from a number of sources and determine if the repeater is still safe to operate

  Sensors
  - Peripheral:
  - - Wind
  - - Temperature
  - - Humidity
  - - Accelerometer
  - Core:
  - - Temperature
  - - Humidity
  - - Battery power

  For transmitting sensor data from top of mast:
  - "<" + SENSOR_CODE_1 + ":" SENSOR_DATA_1 + "," + ... (repeat same format) ... + ">"
  - On initialization of RS485 communication, MASTER will transmit SensorCode enum to slave
*/

#include <string.h>

#include <Arduino.h>
#include "SoftwareSerial.h"
#include <Wire.h>
#include "LiquidCrystal_I2C.h"

#include <ClimateSensor.h>
#include <ControlRelay.h>
#include <ControlLED.h>
#include <BatteryController.h>

#define MIN_TEMP -20
#define MAX_TEMP 80
#define MAX_HUM 80
#define MIN_Z_ACCEL_VALUE 8

#define ssEnable 8
#define ssRX 9
#define ssTX 10

#define SENSOR_BUFFER_SIZE 128

unsigned long previousMillis = 0;
const long pollingInterval = 1000;

char sensorBuffer[SENSOR_BUFFER_SIZE];
int sensorBufferIndex = 0;

bool systemOK = true;
ControlLED::LEDState systemError = ControlLED::OK;

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

SoftwareSerial sensorStream = SoftwareSerial(ssRX, ssTX);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Address, row characters, lines

void setup() {
  Serial.begin(9600);
  sensorStream.begin(9600);

  clim.init();
  cr.init();
  cl.init();

  lcd.init();
  lcd.setCursor(0, 0);
  
  lcd.backlight();
  for (int init = 0; init < 5; init++) {
    lcd.print("DARTCOM, v1.0.0");
    lcd.setCursor(0, 1);

    lcd.print("Loading");
    delay(400);

    for (int p = 0; p < 3; p++) {
      lcd.print(".");
      delay(400);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
  }

  cl.setLEDState(ControlLED::OK);
  cr.enablePower();
  delay(10);
  printSystemStatusToLCD();

  // Potential power saving in the future
  // lcd.clear();
  // lcd.noBacklight();

  pinMode(ssEnable, OUTPUT); // MASTER/SLAVE selector
  delay(10);
  digitalWrite(ssEnable, LOW); // Slave needs enable LOW
}

void loop() {
  unsigned long currentMillis = millis();

  // Non-blocking delay
  if (currentMillis - previousMillis > pollingInterval) {
    previousMillis = currentMillis;
    // checkSystemStatus();

    /** Run local sensor analysis here */
    // TODO: Get local and external checking working simultaneously
    // verifyLocalReadings();
  }

  // TODO: Clean this up into functions
  while (sensorStream.available()) {
    // Get char value from stream
    char sensorStreamTempRead = (char)sensorStream.read();

    // Remove newlines and carrage returns from sensorStream
    if (sensorStreamTempRead != '\n' && sensorStreamTempRead != '\r') {
      sensorBuffer[sensorBufferIndex] = sensorStreamTempRead;

      // If final character received, process then reset
      if (sensorBuffer[sensorBufferIndex] == '>') {

        /**
         * NOTE: Parse each level in separate, non-nested while loops to maintain strtok() consistency
        */

        // ------------- First-level string parsing ------------- //
        // Inspiration: https://forum.arduino.cc/index.php?topic=450585.0

        const int numFirstLevelSplits = 10;
        const int numSecondLevelSplits = 2;

        char *sensorParsedStrings[numFirstLevelSplits]; // Holds all split strings coming in from Serial
        char *sensorParsedStringsHolder; // Char array holder used when parsing incoming data with strtok()
        int sensorIndex = 0; // Holds the maximum filled index in sensorParsedStrings

        sensorParsedStringsHolder = strtok(sensorBuffer, "<,>");

        // Iterate through all first-level splits of incoming char array
        while (sensorParsedStringsHolder != NULL) {
          sensorParsedStrings[sensorIndex] = sensorParsedStringsHolder;

          // Set up for next iteration
          sensorIndex++;
          sensorParsedStringsHolder = strtok(NULL, "<,>");
        }

        // ------------- Second-level string parsing ------------- //
        // Parse through each first-level string and read data into 2D array of ints

        int sensorDataParsed[numFirstLevelSplits][numSecondLevelSplits]; // Saves the id-data pair for each sensor
        char *sensorDataParsedHolder; // String iterator for parsing each sensor substring
        int subSensorIndex = 0; // Index for sensorDataParser

        // Iterate through all first-level and corresponding second-level datasets
        for (int firstLevelIndex = 0; firstLevelIndex < sensorIndex; firstLevelIndex++) {
          
          sensorDataParsedHolder = strtok(sensorParsedStrings[firstLevelIndex], ":");
          subSensorIndex = 0;

          while (sensorDataParsedHolder != NULL) {
            sensorDataParsed[firstLevelIndex][subSensorIndex] = atoi(sensorDataParsedHolder); // Convert to int, save

            subSensorIndex++;
            sensorDataParsedHolder = strtok(NULL, ":");
          }
        }

        // Print all sensor ids and values
        bool foundError = false;

        for (int i = 0; i < sensorIndex; i++) {
          switch(sensorDataParsed[i][0]) {
            case SensorCode::ACCEL_SENSOR_X:
            // Doesn't currently check this value
            Serial.print("X-Acceleration: ");
            break;

            case SensorCode::ACCEL_SENSOR_Y:
            // Doesn't currently check this value
            Serial.print("Y-Acceleration: ");
            break;

            case SensorCode::ACCEL_SENSOR_Z:
              if (sensorDataParsed[i][1] < MIN_Z_ACCEL_VALUE) {
                // TODO: Add fall error to ControlLED
                cl.setLEDState(ControlLED::POWERERROR);
                foundError = true;
              }

              Serial.print("Z-Acceleration: ");
              break;

            case SensorCode::GYRO_SENSOR_X:
              // Doesn't currently check this value
              Serial.print("X-Gyroscope: ");
              break;

            case SensorCode::GYRO_SENSOR_Y:
              // Doesn't currently check this value
              Serial.print("Y-Gyroscope: ");
              break;

            case SensorCode::GYRO_SENSOR_Z:
              // Doesn't currently check this value
              Serial.print("Z-Gyroscope: ");
              break;
            
            case SensorCode::HUMIDITY_SENSOR:
              if (MAX_HUM < sensorDataParsed[i][1]) {
                cl.setLEDState(ControlLED::HUMIDERROR);
                foundError = true;
              }

              Serial.print("Humidity: ");
              break;
            
            case SensorCode::TEMPERATURE_SENSOR:
              if (sensorDataParsed[i][1] < MIN_TEMP && MAX_TEMP < sensorDataParsed[i][1]) {
                cl.setLEDState(ControlLED::TEMPERROR);
                foundError = true;
              }

              Serial.print("Temperature: ");
              break;

            case SensorCode::PRESSURE_SENSOR:
              // Doesn't currently check this value
              Serial.print("Pressure: ");
              break;

            default:
              Serial.print("Other: ");
              break;
          }

          // Serial.print("id: ");
          Serial.print(sensorDataParsed[i][0]);
          Serial.print(", value: ");
          Serial.print(sensorDataParsed[i][1]);
          Serial.println();
        }
        Serial.println("****************");

        sensorBufferIndex = 0; // Reset incoming Serial buffer index

        // If no errors found, set code to OK
        if (foundError == false) {
          cl.setLEDState(ControlLED::OK);
        }

      } else if (sensorBufferIndex >= SENSOR_BUFFER_SIZE - 1) { // Buffer overflow check
        sensorBufferIndex = 0;
        Serial.println("Sensor Buffer Overflow");
        Serial.println(sensorBuffer);
      } else {
        sensorBufferIndex++;
      }
    }
  }

  checkSystemStatus(); // Update relay with any changes to the LED state
}

// Validate sensors attached to core module
void verifyLocalReadings() {
  int temp, humid, press;

  temp = clim.getCurrentTemperature();
  humid = clim.getCurrentHumidity();
  // press = clim.getCurrentPressure();

  // Note: This will determine the relative importance of each error
  if (MAX_HUM < humid) {
    cl.setLEDState(ControlLED::HUMIDERROR);
  } else if (temp < MIN_TEMP || MAX_TEMP < temp) {
    cl.setLEDState(ControlLED::TEMPERROR);
  } else if (!b1.batteryWithinRange() || !b2.batteryWithinRange()) {
    cl.setLEDState(ControlLED::POWERERROR);
  } else {
    cl.setLEDState(ControlLED::OK);
  }
}

// Check if system code is OK, if not disable relay
void checkSystemStatus() {
  if (cl.getLedState() != ControlLED::OK) { // System error

    // Saves printing if systen already knows what's wrong
    if (cl.getLedState() != systemError || systemOK == true) {
      cr.disablePower();

      systemOK = false;
      systemError = cl.getLedState();

      printSystemStatusToLCD();
    }
  } else { // System OK

    // Saves printing if system is already OK
    if (cl.getLedState() != ControlLED::OK || systemOK == false) {
      cr.enablePower();
      
      systemOK = true;
      systemError = ControlLED::OK;

      printSystemStatusToLCD();
    }
  }
}

// Print status code and or any errors to system LCD
void printSystemStatusToLCD() {
  lcd.clear();
  lcd.print(systemOK == true ? "Status" : "Error");
  lcd.print(": ");
  lcd.print(getLEDStateName(cl.getLedState()));
}

// Convert SensorCode to readable string name
const char* getSensorCodeName(SensorCode s) {
  if (s == ACCEL_SENSOR_X) return "Accel[x]";
  else if (s == ACCEL_SENSOR_Y) return "Accel[y]";
  else if (s == ACCEL_SENSOR_Z) return "Accel[z]";
  else if (s == GYRO_SENSOR_X) return "Gyro[x]";
  else if (s == GYRO_SENSOR_Y) return "Gyro[y]";
  else if (s == GYRO_SENSOR_Z) return "Gyro[z]";
  else if (s == HUMIDITY_SENSOR) return "Humidity";
  else if (s == PRESSURE_SENSOR) return "Pressure";
  else if (s == TEMPERATURE_SENSOR) return "Temperature";
  else return "NR";
}

const char* getLEDStateName(ControlLED::LEDState c) {
  if (c == ControlLED::UNDEF) return "Undefined";
    else if (c == ControlLED::SYSTEMERROR) return "System Error";
    else if (c == ControlLED::TEMPERROR) return "Temp Range";
    else if (c == ControlLED::HUMIDERROR) return "Humidity";
    else if (c == ControlLED::POWERERROR) return "Power";
    else if (c == ControlLED::WARNING) return "Warning";
    else if (c == ControlLED::OK) return "Normal";
    else return "NR";
}

// Print status of all core system modules
void printSystemStatus() {
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
  // Serial.print("\t");
  // Serial.print("Pressure: ");
  // Serial.print(press);
  Serial.print("Battery 1 (LiPo): ");
  Serial.print(b1Volt);
  Serial.print("\t");
  Serial.print("Battery 2 (NiMH): ");
  Serial.print(b2Volt);
  Serial.println();
}

// Run through all system codes and cycle relay and LED
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