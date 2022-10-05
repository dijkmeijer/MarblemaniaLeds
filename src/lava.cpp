#include <arduino.h>
#include <OctoWS2811.h>
#include "lava.h"
#include <FastLED.h>

const int ledsPerStrip = 100;

//#define MAP1
#define MAP2

#include "ledmap.h"

#define R 16777215
#define G 65536

LavaEffect::LavaEffect(OctoWS2811 &n_strip, uint8_t _lowStrip)
    : strip(n_strip)
{
  lowStrip = _lowStrip - 1;
  highStrip = _lowStrip;
  ledsPerPin = n_strip.numPixels() / 8;
}

///
/// Set all colors
///
void LavaEffect::Draw()
{
  EVERY_N_MILLISECONDS_I(thistimer, 36)
  {
    // thistimer.setPeriod(random(50, 150));
    Lava(25, 21);
  }
}
///
/// Set color of LED
///
void LavaEffect::AddColor(uint16_t position, uint32_t color)
{
  uint32_t blended_color = Blend(strip.getPixel(position), color);
  strip.setPixelColor(position, blended_color);
}

///
/// Set color of LED
///
void LavaEffect::SubstractColor(uint16_t position, uint32_t color)
{
  uint32_t blended_color = Substract(strip.getPixel(position), color);
  strip.setPixelColor(position, blended_color);
}

///
/// Color blending
///
uint32_t LavaEffect::Blend(uint32_t color1, uint32_t color2)
{
  uint8_t r1, g1, b1;
  uint8_t r2, g2, b2;

  r1 = (uint8_t)(color1 >> 16),
  g1 = (uint8_t)(color1 >> 8),
  b1 = (uint8_t)(color1 >> 0);

  r2 = (uint8_t)(color2 >> 16),
  g2 = (uint8_t)(color2 >> 8),
  b2 = (uint8_t)(color2 >> 0);

  return strip.Color(constrain(r1 + r2, 0, 255), constrain(g1 + g2, 0, 255), constrain(b1 + b2, 0, 255));
}

///
/// Color blending
///
uint32_t LavaEffect::Substract(uint32_t color1, uint32_t color2)
{
  uint8_t r1, g1, b1;
  uint8_t r2, g2, b2;

  int16_t r, g, b;

  r1 = (uint8_t)(color1 >> 16),
  g1 = (uint8_t)(color1 >> 8),
  b1 = (uint8_t)(color1 >> 0);

  r2 = (uint8_t)(color2 >> 16),
  g2 = (uint8_t)(color2 >> 8),
  b2 = (uint8_t)(color2 >> 0);

  r = (int16_t)r1 - (int16_t)r2;
  g = (int16_t)g1 - (int16_t)g2;
  b = (int16_t)b1 - (int16_t)b2;
  if (r < 0)
    r = 0;
  if (g < 0)
    g = 0;
  if (b < 0)
    b = 0;

  return strip.Color(r, g, b);
}

///
/// Every LED to black
///
void LavaEffect::Clear()
{
  // for(uint8_t j=0; j < 8;j++)
  for (int j = lowStrip; j < highStrip; j++)
  {
    int offset = j * ledsPerPin;
    for (int i = 0; i < LAVACOUNT; i++)
      strip.setPixelColor(i + offset, off_color);
  }
}

void LavaEffect::Off()
{
  Clear();
  strip.show();
}

void LavaEffect::Lava(int Cooling, uint8_t Sparking)
{

  // Array of temperature readings at each simulation cell
  // static byte heat[LAVACOUNT];

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < LAVACOUNT; i++)
  {
    heat[i] = qsub8(heat[i], random8(0, ((Cooling * 4) / LAVACOUNT) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = LAVACOUNT - 1; k >= 4; k--)
  {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]+heat[k - 3]+heat[k - 4]) / 5;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < Sparking)
  {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(20, 60));
  }

  // Step 4.  Map from heat cells to LED colors
  for (int j = 0; j < LAVACOUNT; j++)
  {
    uint32_t color = heatColor(heat[j]);
    int pixelnumber;
    // if( gReverseDirection ) {
    //   pixelnumber = (LAVACOUNT-1) - j;
    // } else {
    pixelnumber = led_map[j] + lowStrip * ledsPerStrip - 1;
    // }
    // uint32_t c = (color.g << 16) +  (color.b << 8) + color.r;
    strip.setPixelColor(pixelnumber, color);
  }

  strip.show();
}

uint32_t LavaEffect::heatColor(uint8_t temperature)
{
  uint32_t heatC=0;

  // Scale 'heat' down from 0-255 to 0-191,
  // which can then be easily divided into three
  // equal 'thirds' of 64 units each.
  // uint8_t t192 = scale8_video(temperature, 191);
  uint8_t t192 = scale8_video(temperature, 191);

  // calculate a value that ramps up from
  // zero to 255 in each 'third' of the scale.
  uint8_t heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2;                 // scale up to 0..252

  // now figure out which third of the spectrum we're in:
  if (t192 & 0x80)
  {
    // we're in the hottest third
    // heatcolor = R + G + heatramp;
    // full red
    heatC = 0;
    // heatcolor = R | G | heatramp; // full red
    // heatcolor.g = 255; // full green
    // heatcolor.b = heatramp; // ramp up blue
  }
  else if (t192 & 0x40)
  {
    // we're in the middle third
    heatC = R + (heatramp);
    // heatcolor.r = 255; // full red
    // heatcolor.g = heatramp; // ramp up green
    // heatcolor.b = 0; // no blue
  }
  else
  {
    // we're in the coolest third
    heatC = (heatramp << 16);
    // heatcolor.r = heatramp; // ramp up red
    // heatcolor.g = 0; // no green
    // heatcolor.b = 0; // no blue
  }

  return heatC;
}