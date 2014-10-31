/*
  Watch
 
 created 2014
 by Otmar Klenk
 
 This example code is in the public domain.
 */

//#include <LowPower.h>
#include <G7th.h>

int lastSecs = 0;


void setup() {
  // initialize Serial class
  Serial.begin( 9600 );

  // use timer 1
  useTimer1();         // short for useTimer1( true )
  hms( 14, 45, 00 );   // set the time to 14:45:00
                       // needs to be done from outside
}


void loop() {
  // ATmega328P, ATmega168
  //LowPower.idle( SLEEP_30MS, ADC_OFF, TIMER2_OFF, TIMER1_ON, TIMER0_OFF, 
  //               SPI_OFF, USART0_ON, TWI_OFF );

  // ATmega32U4
  //LowPower.idle( SLEEP_30MS, ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_ON, 
  //		   TIMER0_OFF, SPI_OFF, USART1_ON, TWI_OFF, USB_OFF );

  // ATmega2560
  //LowPower.idle( SLEEP_30MS, ADC_OFF, TIMER5_OFF, TIMER4_OFF, TIMER3_OFF, 
  //		   TIMER2_OFF, TIMER1_ON, TIMER0_OFF, SPI_OFF, USART3_OFF, 
  //		   USART2_OFF, USART1_OFF, USART0_ON, TWI_OFF );

  int secs = seconds;          // get the actual seconds
  if( lastSecs != secs ) {   
    lastSecs = secs;
    Serial.println( hms() );   // print the actual time
  }
}

