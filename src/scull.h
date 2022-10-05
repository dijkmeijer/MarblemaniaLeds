#ifndef SCULL_H
#define SCHULL_H

#include "arduino.h"
#include <OctoWS2811.h>
#include <fastLED.h>

#define SCULLCOUNT 90
// Led strip is coded GBR
#define RED 0x0000FF
#define GREEN 0xFF0000
#define BLUE 0x00FF00
#define WHITE 0x555555

class ScullEffect
{
    OctoWS2811 &strip;
    int ledsPerPin;
    uint8_t lowStrip;
    uint8_t highStrip;
    uint32_t fire_color = 0x0000FF;
    uint32_t off_color = 0x000000;
    uint32_t rot;

public:
    ScullEffect(OctoWS2811 &, uint8_t, uint8_t);
    void Draw();
    void Draw(uint32_t);
    void Clear();
    void Off();
    void Rotate();
    void AddColor(uint16_t position, uint32_t color);
    void SubstractColor(uint16_t position, uint32_t color);
    uint32_t Blend(uint32_t color1, uint32_t color2);
    uint32_t Blend(uint32_t color1, uint32_t color2, float fraction);
    uint32_t Substract(uint32_t color1, uint32_t color2);
};


#endif
