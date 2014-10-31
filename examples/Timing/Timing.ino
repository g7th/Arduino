/*
  Button
 
 Turns on and off a LED (light emitting diode) connected to digital pin 13
 and toggles the on/off state of a second LED connected to digital pin 7
 simultaneously, while pressing a pushbutton attached to pin 2.
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * LED attached from pin 7 to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 
 created 2014
 by Otmar Klenk
 
 This example code is in the public domain.
 */

#include <G7th.h>


Button button = Button( 2, HIGH );  // set button pin 2 on active HIGH
const int led1Pin = 13;             // the pin number of the first LED
const int led2Pin = 7;              // the pin number of the second LED
unsigned long tm;                   // variable to store press time 0
                                    

void setup() {
  // button pin 2 get initialized automaticly as input

  // initialize the LED pins as an output:
  pinMode( led1Pin, OUTPUT );
  pinMode( led2Pin, OUTPUT );

  // initialize Serial class
  Serial.begin( 9600 );
}

void loop(){
  // check if the pushbutton is pressed
  // ...and switch the LED 1
  digitalWrite( led1Pin, pressed( button ) );
  // ...and switch the LED 2
  digitalWrite( led2Pin, clicked( button ) );

  // how many times, how long etc. was the pushbutton pressed?
  if( tm != pressTime0( button ) ) {  // check on repeated values to prevent
                                      // an overrun of the serial port
    tm = pressTime0( button );
    Serial.print( "press time  reseted value on release: " );
    Serial.print( tm );
    Serial.print( "  captured value on release: " );
    Serial.print( pressTime( button ) );
    Serial.print( "  the button was pressed " );
    Serial.print( pushes( button ) );
    Serial.print( " time(s)" );
    Serial.println( isPressed( button ) ? "  button is pressed" : "  button is  n o t  pressed" );

    // state seen on second LED:
    Serial.print( "click time  on time: " );
    Serial.print( onTime( button ) );
    Serial.print( "  off time: " );
    Serial.print( offTime( button ) );
    Serial.print( "  the button was clicked " );
    Serial.print( clicks( button ) );
    Serial.print( " time(s)" );
    Serial.println( isClicked( button ) ? "  button is clicked" : "  button is  n o t  clicked" );
  }
}

