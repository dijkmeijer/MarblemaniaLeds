#ifndef LAVA_H
#define LAVA_H

#include <arduino.h>
#include <OctoWS2811.h>
#include "FastLED.h"

#define LAVACOUNT 100

class LavaEffect
{
    OctoWS2811 &strip;
    int ledsPerPin;
    uint8_t lowStrip;
    uint8_t highStrip;
    uint32_t fire_color = 0xff0022;
    uint32_t off_color = 0x000000;
    uint8_t heat[LAVACOUNT];

public:
    LavaEffect(OctoWS2811 &, uint8_t);
    void Draw();
    void Off();
    void Clear();
    void Lava(int Cooling, uint8_t Sparking);
    uint32_t heatColor( uint8_t temperature);
    void setPixelHeatColor(int Pixel, byte temperature);
    void AddColor(uint16_t position, uint32_t color);
    void SubstractColor(uint16_t position, uint32_t color);
    uint32_t Blend(uint32_t color1, uint32_t color2);
    uint32_t Substract(uint32_t color1, uint32_t color2);
};

#endif