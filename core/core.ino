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

#include <string.h>

#include <Arduino.h>
#include "SoftwareSerial.h"

#include <ClimateSensor.h>
#include <ControlRelay.h>
#include <ControlLED.h>
#include <BatteryController.h>
#include <MotionDetection.h>

// TODO: Serve this to all SLAVE boards
#define MIN_TEMP -20
#define MAX_TEMP 80
#define MAX_HUM 80

#define ssEnable 8
#define ssRX 9
#define ssTX 10

#define SENSOR_BUFFER_SIZE 128

unsigned long previousMillis = 0;
const long pollingInterval = 1000;

char sensorBuffer[SENSOR_BUFFER_SIZE];
int sensorBufferIndex = 0;

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

SoftwareSerial sensorStream = SoftwareSerial(ssRX, ssTX);

void setup() {
  Serial.begin(9600);
  sensorStream.begin(9600);

  clim.init();
  cr.init();
  cl.init();
  imu.init();

  pinMode(ssEnable, OUTPUT); // MASTER/SLAVE selector
  delay(10);
  digitalWrite(ssEnable, LOW); // Slave needs enable LOW

  cr.enablePower();
  // delay(2000);
  // systemCycleTest();
}

void loop() {
  unsigned long currentMillis = millis();

  // Non-blocking delay
  if (currentMillis - previousMillis > pollingInterval) {
    previousMillis = currentMillis;
    // checkSystemStatus();

    /** Run local sensor analysis here */
  }

  while (sensorStream.available()) {
    // Get char value from stream
    char sensorStreamTempRead = (char)sensorStream.read();

    // Remove newlines and carrage returns from sensorStream
    if (sensorStreamTempRead != '\n' && sensorStreamTempRead != '\r') {
      sensorBuffer[sensorBufferIndex] = sensorStreamTempRead;

      // If final character received, process then reset
      if (sensorBuffer[sensorBufferIndex] == '>') {

        /** Process data here */

        /**
         * NOTE: Parse each level in separate, non-nested while loops to maintain strtok() consistency
        */

        // ------------- First-level string parsing ------------- //
        // Inspiration: https://forum.arduino.cc/index.php?topic=450585.0

        // TODO: Make this more coherent
        const int firstLevelSplits = 10;
        const int secondLevelSplits = 2;

        char *sensorParsedStrings[firstLevelSplits]; // TODO: Document this variable
        char *sensorParsedStringsTemp; // TODO: Document this variable
        int sensorIndex = 0; // TODO: Document this variable

        // TODO: Rename this variable
        sensorParsedStringsTemp = strtok(sensorBuffer, "<,>");

        // Iterate through all first-level splits of incoming char array
        while (sensorParsedStringsTemp != NULL) {
          sensorParsedStrings[sensorIndex] = sensorParsedStringsTemp;

          // Set up for next iteration
          sensorIndex++;
          sensorParsedStringsTemp = strtok(NULL, "<,>");
        }

        // ------------- Second-level string parsing ------------- //
        // Parse through each first-level string and read data into 2D array of ints

        Serial.print("Sensor index: ");
        Serial.println(sensorIndex);

        int sensorDataParser[firstLevelSplits][secondLevelSplits]; // Saves the id-data pair for each sensor
        char *sensorDataParserTemp; // String iterator for parsing each sensor substring
        int subSensorIndex = 0; // Index for sensorDataParser

        // Iterate through all first-level and corresponding second-level datasets
        for (int firstLevelIndex = 0; firstLevelIndex < sensorIndex; firstLevelIndex++) {
          
          // TODO: Rename this variable
          sensorDataParserTemp = strtok(sensorParsedStrings[firstLevelIndex], ":");
          subSensorIndex = 0;

          while (sensorDataParserTemp != NULL) {
            // Serial.println(sensorDataParserTemp);
            // Serial.println((int)(*sensorDataParserTemp) - 48);
            // Serial.println((int)(*sensorDataParserTemp) - '0');
            // Serial.println("--");
            sensorDataParser[firstLevelIndex][subSensorIndex] = (int)(*sensorDataParserTemp) - '0'; // Dereference, convert to int

            // Serial.println(sensorDataParser[firstLevelIndex][subSensorIndex]);
            // Serial.println("----");

            subSensorIndex++;
            sensorDataParserTemp = strtok(NULL, ":");
          }
        }

        Serial.println("Begin array iteration");
        for (int i = 0; i < sensorIndex; i++) {
          for (int j = 0; j < secondLevelSplits; j++) {
            // Serial.print(i);
            // Serial.print("\t");
            // Serial.print(j);
            // Serial.print("\t");
            Serial.println(sensorDataParser[i][j]);
          }
          Serial.println("----");
        }
        Serial.println("**************");
        
        // Serial.println(sensorBuffer);

        sensorBufferIndex = 0; // Reset buffer index
      } else if (sensorBufferIndex >= SENSOR_BUFFER_SIZE - 1) { // Buffer overflow check
        sensorBufferIndex = 0;
        Serial.println("Sensor Buffer Overflow");
        Serial.println(sensorBuffer);
      } else {
        sensorBufferIndex++;
      }
    }
  }
}

// TODO: Make these functions into exteral libraries
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