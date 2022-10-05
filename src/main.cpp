#include <arduino.h>
#include <OctoWS2811.h>
#include <fastLED.h>
#include "flame.h"
#include "scull.h"
#include "lava.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * Marblemania led effects
 * v. 1.0
 * Copyright (C) 2015 Valentijn Dijkmeijer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define LEDSPERPIN 100

#define GREEN 0xFF0000
#define BLUE 0x00FF00
#define RED 0x0000FF


#define PLC_1 17
#define PLC_2 18
#define PLC_3 19

int thistimer;
#define READPLC ((0x1 & !digitalRead(PLC_1)) | ((0x1 & !digitalRead(PLC_2)) << 1) | ((0x1 & !digitalRead(PLC_3)) << 2))

const int ledsPerPin = LEDSPERPIN;
DMAMEM int displayMemory[ledsPerPin * 6];
int drawingMemory[ledsPerPin * 6];
const int config = WS2811_RBG | WS2811_800kHz;
OctoWS2811 strip(ledsPerPin, displayMemory, drawingMemory, config);

FlameEffect flame(strip, 5, 7);
ScullEffect scull(strip, 2, 3);
LavaEffect lava(strip, 4);

uint8_t PLC_status;

///
/// Setup
///
void setup()
{
  pinMode(PLC_1, INPUT_PULLUP);
  pinMode(PLC_2, INPUT_PULLUP);
  pinMode(PLC_3, INPUT_PULLUP);

  //
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

///
/// Main loop
///
void loop()
{

  PLC_status = READPLC;
  
  // scull.Draw(WHITE);
  switch (PLC_status)
  {
  case 0:
    lava.Off();
    flame.Off();
    scull.Draw(GREEN);
    // scull.Rotate();
    break;
  case 1:
    lava.Draw();
    flame.Draw();
    scull.Draw(BLUE);
    break;
  case 2:
    scull.Draw(RED);
    flame.Draw();
    lava.Draw();
    break;
  case 3:
    scull.Rotate();
    flame.Draw();
    lava.Draw();
    break;
  case 4:
    scull.Draw(GREEN);
    flame.Off();
    lava.Draw();
    break;
  case 5:
    scull.Draw(BLUE);
    flame.Off();
    lava.Off();
    break;
  case 6:
    scull.Draw(RED);
    flame.Draw();
    lava.Off();
    break;
  case 7:
    scull.Draw(GREEN);
    flame.Off();
    lava.Off();
    break;
  }
  
  // Serial.println(strip.numPixels());
  // delay(random(50, 150));

}