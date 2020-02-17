/*
 * Creating an engine to process input from a number of sources and determine if the repeater is still safe to operate
 * 
 * Sensors
 * - Top:
 * - - Wind
 * - - Temperature
 * - - Humidity
 * - Bottom:
 * - - Temperature
 * - - Humidity
 * - - Battery power
 */

#include <Humidity.h>
#include <Temperature.h>
#include <Wind.h>

void setup() {
  Serial.begin(9600);
  
  Temperature temp = Temperature();
  Humidity hum = Humidity();
  Wind wind = Wind();

  temp.init(1.00);
  hum.init(1.00);
  wind.init(1.00);

  Serial.println(temp.getSafeLimit());
  Serial.println(hum.getSafeLimit());
  Serial.println(wind.getSafeLimit());
}

void loop() {
  // put your main code here, to run repeatedly:

}
