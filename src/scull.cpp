#include "scull.h"
#include "RColor.h"

ScullEffect::ScullEffect(OctoWS2811 &n_strip, uint8_t _lowStrip, uint8_t _highStrip)
    : strip(n_strip)
{
    ledsPerPin = n_strip.numPixels() / 8;
    lowStrip = _lowStrip - 1;
    highStrip = _highStrip;
    rot = 0;
}

///
/// Set all colors
///
void ScullEffect::Draw()
{

    for (int j = lowStrip; j < highStrip; j++)
    {
        int offset = j * ledsPerPin;
        for (int i = 0; i < SCULLCOUNT; i++)
        {
            //             AddColor(i + offset, fire_color);
            //             int r = random(255);

            //             uint32_t diff_color = strip.Color(r, r / 2, r / 2);
            //             SubstractColor(i + offset, diff_color);
            strip.setPixelColor(i + offset, Blend(0x000055, 0x550000, i / 90.0));
        }
    }
    strip.show();
}

void ScullEffect::Draw(uint32_t _color)
{

    for (int j = lowStrip; j < highStrip; j++)
    {
        int offset = j * ledsPerPin;
        for (int i = 0; i < SCULLCOUNT; i++)
        {

            strip.setPixelColor(i + offset, _color);
        }
    }
    strip.show();
}

void ScullEffect::Rotate()
{
    EVERY_N_MILLISECONDS_I(thistimer, 20)
    {
        rot++;
        for (int j = lowStrip; j < highStrip; j++)
        {
            int offset = j * ledsPerPin;
            for (int i = 0; i < SCULLCOUNT; i++)
            {
                strip.setPixelColor(i + offset, RotateColor[(i + rot) % SCULLCOUNT]);
            }
        }
        strip.show();
    }
}

///
/// Set color of LED
///
void ScullEffect::AddColor(uint16_t position, uint32_t color)
{
    uint32_t blended_color = Blend(strip.getPixel(position), color);
    strip.setPixelColor(position, blended_color);
}

///
/// Set color of LED
///
void ScullEffect::SubstractColor(uint16_t position, uint32_t color)
{
    uint32_t blended_color = Substract(strip.getPixel(position), color);
    strip.setPixelColor(position, blended_color);
}

///
/// Color blending
///
uint32_t ScullEffect::Blend(uint32_t color1, uint32_t color2)
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

uint32_t ScullEffect::Blend(uint32_t color1, uint32_t color2, float fraction)
{

    uint8_t r1, g1, b1;
    uint8_t r2, g2, b2;
    uint8_t f1 = 255 * fraction;
    uint8_t f2 = 255 - (255 * fraction);

    r1 = map((uint8_t)(color1 >> 16), 0, 255, 0, f1);
    g1 = map((uint8_t)(color1 >> 8), 0, 255, 0, f1);
    b1 = map((uint8_t)(color1 >> 0), 0, 255, 0, f1);

    r2 = map((uint8_t)(color2 >> 16), 0, 255, 0, f2);
    g2 = map((uint8_t)(color2 >> 8), 0, 255, 0, f2);
    b2 = map((uint8_t)(color2 >> 0), 0, 255, 0, f2);

    //    return strip.Color(constrain(r1 + r2, 0, 255), constrain(g1 + g2, 0, 255), constrain(b1 + b2, 0, 255));
    return strip.Color(r1 + r2, g1 + g2, b1 + b2);
}

///
/// Color blending
///
uint32_t ScullEffect::Substract(uint32_t color1, uint32_t color2)
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
void ScullEffect::Clear()
{
    // for(uint8_t j=0; j < 8;j++)
    for (int j = lowStrip; j < highStrip; j++)
    {
        int offset = j * ledsPerPin;
        for (int i = 0; i < SCULLCOUNT; i++)
            strip.setPixelColor(i + offset, off_color);
    }
}

void ScullEffect::Off()
{
    Clear();
    strip.show();
}
