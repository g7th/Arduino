/*
  Button
 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 13, when pressing a pushbutton attached to pin 2. 
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 
 created 2014
 by Otmar Klenk
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Button
 */

#include <G7th.h>


Button button = Button( 2, HIGH );  // set button pin 2 on active HIGH
const int ledPin = 13;              // the number of the LED pin


void setup() {
  // button pin 2 get initialized automaticly as input

  // initialize the LED pin as an output:
  pinMode( ledPin, OUTPUT );
}

void loop(){
  // check if the pushbutton is pressed
  // and switch the LED
  digitalWrite( ledPin, pressed( button ) );
}

