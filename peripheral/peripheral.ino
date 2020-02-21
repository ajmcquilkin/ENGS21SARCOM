#include <Arduino.h>
#include <ClimateSensor.h>
#include <MotionDetection.h>

// TODO: Serve this to all SLAVE boards
#define MIN_TEMP -20
#define MAX_TEMP 80
#define MAX_HUM 80

// TODO: Load this from MASTER node
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

void setup() {

}

void loop() {

}