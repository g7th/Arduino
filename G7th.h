/*
 *  G7th.h - Library for using Buttons.
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

#ifndef _G7TH_H
#define _G7TH_H

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define counter1 ( F_CPU / 1000 - 1 )


class Button {
public:
  Button( int pin, int state, boolean isClicked, int mode );
  Button( int pin, int state, boolean isClicked = false );

  unsigned long press_tm;
  unsigned long on_tm;
  unsigned long off_tm;

  friend bool isPressed( Button& bn );
  friend bool isClicked( Button& bn );
  friend bool isDoubleClicked( Button& bn, unsigned long speed );
  friend int pressed( Button& bn );
  friend int clicked( Button& bn );
  friend int pushes( Button& bn );
  friend int pushes( Button& bn, int start );
  friend int clicks( Button& bn );
  friend int clicks( Button& bn, int start );
  friend unsigned long pressTime0( Button& bn );
  friend unsigned long pressTime( Button& bn );
  friend unsigned long onTime( Button& bn );
  friend unsigned long offTime( Button& bn );
  friend void pinMode( Button& bn, int mode );
  friend int digitalRead( Button& bn );

protected:
  uint8_t pin;
  int state;
  int mode;
  bool is_pressed;
  bool was_clicked;
  int pushes;
  int clicks;
  int presses;
  int releases;
  unsigned long press_tm0;
  unsigned long on_tm0;
  unsigned long off_tm0;
  bool isPressed();

private:
  unsigned long duration_tm0;
};


extern volatile int hours;
extern volatile int minutes;
extern volatile int seconds;

extern bool isPressed( Button& bn );
extern bool isClicked( Button& bn );
extern bool isDoubleClicked( Button& bn, unsigned long speed );
extern int pressed( Button& bn );
extern int clicked( Button& bn );
extern int pushes( Button& bn );
extern int pushes( Button& bn, int start );
extern int clicks( Button& bn );
extern int clicks( Button& bn, int start );
extern unsigned long pressTime0( Button& bn );
extern unsigned long pressTime( Button& bn );
extern unsigned long onTime( Button& bn );
extern unsigned long offTime( Button& bn );
extern void pinMode( Button& bn, int mode );
extern int digitalRead( Button& bn );
extern void togglePin( int pin );
extern void togglePin( int pin, unsigned long speed );
extern bool useTimer1( bool b = true );
extern String hms( int h, int m, int s );
extern String hms();
extern String hm();
extern unsigned long hmsToSeconds();

#endif //_G7TH_H
