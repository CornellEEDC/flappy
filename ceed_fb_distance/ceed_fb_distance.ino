/*
Library to interface with a distance sensor and buzzer.

Refer to the DYP-R01CW documentation for distance sensor wiring.
The buzzer must be connected to a digital PWM pin ('~' pins) and GND.
*/


#include <Wire.h>
#include <DYP_R01CW.h> //You will need to install the DYP-R01CW library



/**
* If the distance measured by `sensor`, connected to `buzzerPinNumber`, is less than `distanceRange` (in mm), plays a sad tune and returns true.
* Otherwise, returns false.
* 
* @param distanceRange distance from sensor, in mm, to trigger the tune. Must be non-negative
* @param sensor DYP_R01CW sensor object to read from
* @param buzzerPinNumber pin number that the buzzer is connected to. Must be a digital PWM pin (i.e. 3, 5, 6, 9, 10, 11)
* @param printingDistanceToSerial whether to print distance to Serial. Serial must begin at 115200 baud. Default: false
* @return whether there is an object wihin `distanceRange` millimeters from the distance sensor
*/
bool checkDistance(int16_t distanceRange, const DYP_R01CW& sensor, uint8_t buzzerPinNumber, bool printingDistanceToSerial = false);



////////////////////////////////////////////////
//TESTING VARIABLES

const uint8_t BUZZER_PIN = 11;

DYP_R01CW sensor;

////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);

  if(!sensor.begin()) {
    Serial.println("could not find sensor");
    delay(250);
  }
}


void loop() {

  if(checkDistance(-1, sensor, BUZZER_PIN, true)) {
    return;
  }

  delay(10); 
}


/**
* If the distance measured by `sensor`, connected to `buzzerPinNumber`, is less than `distanceRange` (in mm), plays a sad tune and returns true.
* Otherwise, returns false.
* 
* @param distanceRange distance from sensor, in mm, to trigger the tune. Must be non-negative
* @param sensor DYP_R01CW sensor object to read from
* @param buzzerPinNumber pin number that the buzzer is connected to. Must be a digital PWM pin (i.e. 3, 5, 6, 9, 10, 11)
* @param printingDistanceToSerial whether to print distance to Serial. Serial must begin at 115200 baud. Default: false
* @return whether there is an object wihin `distanceRange` millimeters from the distance sensor
*/
bool checkDistance(int16_t distanceRange, const DYP_R01CW& sensor, uint8_t buzzerPinNumber, bool printingDistanceToSerial = false) {
  int16_t distanceFromSensor = sensor.readDistance(); //Returns distance in mm

  if(printingDistanceToSerial) {
    Serial.print("distance from sensor: ");
    Serial.print(distanceFromSensor);
    Serial.println("mm");
  }

  if(distanceFromSensor <= distanceRange) {
    //Notes played: Bb A Ab G
    tone(buzzerPinNumber, 233);
    delay(500);
    tone(buzzerPinNumber, 220);
    delay(500);
    tone(buzzerPinNumber, 207);
    delay(500);
    tone(buzzerPinNumber, 196);
    delay(1000);
    noTone(buzzerPinNumber);

    return true;
  }

  return false;
}