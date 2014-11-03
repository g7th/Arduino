/*
 *  G7th.cpp - Library for using Buttons.
 *  Copyright (c) 2014 7th Stone Software GmbH. All rights reserved.
 *  Created by Otmar Klenk, October 15, 2014.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  See file LICENSE.txt for further informations on licensing terms.
 */

#include "G7th.h"


inline unsigned long csec( unsigned long ms ) {
  unsigned long q = ms * 13L;
  return( (unsigned long)(((q >> 1) + ((q - (q >> 6)) >> 1)) >> 7) );
}

volatile int hours = 0;
volatile int minutes = 0;
volatile int seconds = 0;

volatile unsigned long timer1_millis = 0;
volatile int msecs = 0;

ISR( TIMER1_COMPB_vect ) {
  unsigned long r = timer1_millis;
  r++; timer1_millis = r;
  if( (msecs++) >= 999 ) {
    msecs = 0;
    if( (seconds++) >= 59 ) {
      seconds = 0;
      if( (minutes++) >= 59 ) {
        minutes = 0;
        if( (hours++) >= 23 ) {
          hours = 0;
        }
      }
    }
  }
}

bool use_tmr1 = false;

inline unsigned long _millis() {
  unsigned long rslt;
  if( use_tmr1 ) {
    uint8_t sreg = SREG;
    cli();
    rslt = timer1_millis;
    SREG = sreg;
  }
  else {
    rslt = millis();
  }
  return( rslt );
}

inline unsigned long delta( unsigned long tm0 ) {
  return( _millis() - tm0 );
}


Button::Button( int pin, int state, boolean isClicked, int mode )
  : is_pressed( false ),
    pushes( 0 ),
    clicks( 0 ),
    presses( 0 ),
    releases( 0 ),
    press_tm0( _millis() ),
    press_tm( 0 ),
    on_tm0( _millis() ),
    on_tm( 0 ),
    off_tm0( _millis() ),
    off_tm( 0 ),
    duration_tm0( _millis() )
{
  this->pin = (uint8_t)pin;
  this->state = state;
  this->was_clicked = isClicked;
  pinMode( this->pin, this->mode = mode );
}

Button::Button( int pin, int state, boolean isClicked )
  : is_pressed( false ),
    pushes( 0 ),
    clicks( 0 ),
    presses( 0 ),
    releases( 0 ),
    press_tm0( _millis() ),
    press_tm( 0 ),
    on_tm0( _millis() ),
    on_tm( 0 ),
    off_tm0( _millis() ),
    off_tm( 0 ),
    duration_tm0( _millis() )
{
  this->pin = (uint8_t)pin;
  this->state = state;
  this->was_clicked = isClicked;
  pinMode( this->pin, this->mode = state != HIGH ? INPUT_PULLUP : INPUT );
}

bool Button::isPressed() {
  boolean isActive;
  while( delta( duration_tm0 ) <= 5 );
  duration_tm0 = _millis();
  if( (isActive = (digitalRead( pin ) == state)) ? (presses++ - releases) >= 5
      : (releases++ - presses) >= 5 ) {
    if( is_pressed != isActive ) {
      if( (is_pressed = isActive) == true ) {
        pushes++;
        if( was_clicked = ! was_clicked ) {
          clicks++;
        }
        press_tm0 = on_tm0 = off_tm0 = _millis();
      }
    }
    presses = 0;
    releases = 0;
  }
  if( is_pressed ) {
    press_tm = delta( press_tm0 );
  }
  if( was_clicked ) {
    on_tm = delta( on_tm0 );
  }
  else {
    off_tm = delta( off_tm0 );
  }
  return( is_pressed );
}

bool isPressed( Button& bn ) {
  return( bn.isPressed() );
}

bool isClicked( Button& bn ) {
  bn.isPressed();
  return( bn.was_clicked );
}

bool isDoubleClicked( Button& bn, unsigned long speed ) {
  if( bn.isPressed() )
    return( csec( bn.on_tm + bn.off_tm ) <= speed );
  return( false );
}

int pressed( Button& bn ) {
  return( bn.isPressed() ? HIGH : LOW );
}

int clicked( Button& bn ) {
  bn.isPressed();
  return( bn.was_clicked ? HIGH : LOW );
}

int pushes( Button& bn ) {
  bn.isPressed();
  return( bn.pushes );
}

int pushes( Button& bn, int start ) {
  int rslt = bn.pushes;
  bn.pushes = start;
  return( rslt );
}

int clicks( Button& bn ) {
  bn.isPressed();
  return( bn.clicks );
}

int clicks( Button& bn, int start ) {
  int rslt = bn.clicks;
  bn.clicks = start;
  return( rslt );
}

unsigned long pressTime0( Button& bn ) {
  if( bn.isPressed() )
    return( csec( bn.press_tm ) );
  return( 0 );
}

unsigned long pressTime( Button& bn ) {
  bn.isPressed();
  return( csec( bn.press_tm ) );
}

unsigned long onTime( Button& bn ) {
  bn.isPressed();
  return( csec( bn.on_tm ) );
}

unsigned long offTime( Button& bn ) {
  bn.isPressed();
  return( csec( bn.off_tm ) );
}

void pinMode( Button& bn, int mode ) {
  pinMode( bn.pin, bn.mode = mode );
}

int digitalRead( Button& bn ) {
  return( digitalRead( bn.pin ) );
}


struct led_t {
   int pin;
   unsigned long tm0;
   unsigned long toggle_tm;
   led_t* next;
};

led_t* led_hook = NULL;

led_t* getLed( int pin ) {
  led_t** next = &led_hook;
  led_t* rslt;
  do
  {
    rslt = *next;
    if( rslt == NULL ) {
      rslt = *next = new led_t;
      if( rslt == NULL ) {
        return( NULL );
      }
      pinMode( rslt->pin = pin, OUTPUT );
      rslt->tm0 = _millis();
      rslt->toggle_tm = -1;
      rslt->next = NULL;
    }
    next = &rslt->next;
  } while( rslt->pin != pin );
  return( rslt );
}

void togglePin( int pin ) {
  togglePin( pin, 80 );
}

void togglePin( int pin, unsigned long speed ) {
  led_t* led = getLed( pin );
  if( led != NULL ) {
    if( digitalRead( pin ) == LOW ) {
      if( led->toggle_tm > (speed * 10L) - (speed << 1) ) {
        led->tm0 = _millis();
        led->toggle_tm = 0;
        led->pin = pin;
        digitalWrite( pin, HIGH );
      }
      else {
        led->toggle_tm = delta( led->tm0 );
      }
    }
    else {
      if( led->toggle_tm > (speed << 1) ) {
        led->toggle_tm = 0;
        digitalWrite( pin, LOW );
      }
      else {
        led->toggle_tm = delta( led->tm0 );
      }
    }
  }
}


bool useTimer1( bool b ) {
  if( use_tmr1 != b ) {
    if( (use_tmr1 = b) == true ) {
      unsigned long ms = millis();
      uint8_t sreg = SREG;
      cli();
      TCCR1A = 0;  // PWM off
      TCCR1B = _BV(WGM12)|_BV(CS10);  // CTC / no prescaler
      TCNT1 = 0;
      OCR1A = counter1;
      TIMSK1 = _BV(OCIE1B);
      timer1_millis = ms;
      msecs = 0;
      SREG = sreg;
    }
  }
}

String hms( int h, int m, int s ) {
  msecs = 0;
  seconds = (s % 60);
  minutes = (m % 60);
  hours = (h % 24);
  return( hms() );
}


int hm_s = 0;

String hm() {
  int h, m;
  do
  {
    hm_s = seconds;
    m = minutes;
    h = hours;
  } while( hm_s != seconds || m != minutes || h != hours );
  String rslt = String();
  h = abs( h );
  m = abs( m );
  if( h < 10 )
    rslt += ' ';
  rslt += h;
  rslt += m < 10 ? ":0" : ":";
  rslt += m;
  return( rslt );
}

String hms() {
  String rslt = hm();
  rslt += hm_s < 10 ? ":0" : ":";
  rslt += hm_s;
  return( rslt );
}

unsigned long hmsToSeconds() {
  int h, m, s;
  do
  {
    s = seconds;
    m = minutes;
    h = hours;
  } while( s != seconds || m != minutes || h != hours );
  return( h * 3600L + m * 60L + s );
}
