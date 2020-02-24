# ENGS 21 Search and Rescue Communication

Building better search and rescue communication - ENGS 21, Dartmouth College, 20W

## Loading Project

- Download Arduino from [Arduino.cc](https://www.arduino.cc/)
- Download VSCode
- Clone _ENGS21SARCOM_ repo
- Copy all sub-library folders (e.g. Wind, Temperature, Sensor, etc...) to Arduino's root _library_ folder
- Wire two microcontrollers together with RS485 controllers and attach any requisite sensors to the two boards
- Run `Core.ino` on one microcontroller, and run `Peripheral.ino` on another

## Electronics Setup

An outline of the electronic systems used by the system to detect any anomalies within the system and respond to protect the battery packs and downstream electrical systems.

### Peripheral Microcontroller

This microcontroller (_Arduino Uno_ at the moment) will fit within the following specifications:

- Equipped with the following sensors measuring the surrounding environment:
  - Atmospheric pressure sensor
  - Humidity sensor
  - Atmospheric temperature sensor
  - Accelerometer / gyroscope module

- Read these sensors at regular intervals

- Transcode sensor protocols (SPI, I2C, other) into RS485 and transmit onto wire using UART

- Detect anomalies in any **urgent** sensor values and alert the **core microcontroller**

- Transmit data down to the **core microcontroller** via an adapted RS485 protocol

  - Transmit this data on a regular basis for **non-urgent** sensor readings (temperaure, pressure, humidity, normal accelerometer / gyroscope readings)
  - Transmit any **urgent** sensor readings (anomalous accelerometer / gyroscope readings) as sensor values update

### Core Microcontroller

This microcontroller (_Arduino Uno_ at the moment) will fit within the following specifications:

- Equipped with the following sensors placed within the battery holder of the system:
  - Atmospheric pressure sensor
  - Humidity sensor
  - Atmospheric temperature sensor

- Read these sensors at regular intervals
  - If any collected data are above set _safe_ threshold values, respond accordingly

- Detect any anomalies in any **urgent** sensor values and respond accordingly

- Read data from the **peripheral microcontroller** as it becomes available via an adapted RS485 protocol

  - Poll data from the RS485 bus and determine if any of the data are above threshold levels
  - If such data are above threshold values, react accordingly

### Power Relay and Indication System

This system is separate from but controlled by the **core microcontroller**, and will fit within the following specifications:

- Have the following physical modules with data buses to the **core microcontroller**:

  - Relay module interrupting the positive power supply line from the battery packs to the main relay module
  - RGB status LED with pre-defined states and corresponding display colors

- Be able to interrupt power on any fault detected by the **core microcontroller** or **peripheral microcontroller**

- Be able to signal quickly which fault has been detected by the **core microcontroller** or **peripheral microcontroller** to a technician or user
