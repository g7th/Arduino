/*
  Blinkenlite

 Turns on and off 8 light emitting diodes (LEDs) connected to
 the digital pins 0 to 7. 

 The circuit:
 * each LED attached from one of the pins (0-7)
 * over a 330Ω resistor to ground
 
 created 2014
 by Otmar Klenk
 
 This example code is in the public domain.
 */

#include <G7th.h>


// the setup function runs once when you press reset or power the board
void setup() {
  for( int i = 0; i < 8; i++ ) {
    delay( 60 );        // keep delay out of the main loop
                        // prevents that all leds are flashing
                        // at the same time
    togglePin( i, 60 ); // no need to call pinMode( i, OUTPUT );
  }
}

// the loop function runs over and over again forever
void loop() {
  for( int i = 0; i < 8; i++ ) {
    togglePin( i, 60 );
  }
}
