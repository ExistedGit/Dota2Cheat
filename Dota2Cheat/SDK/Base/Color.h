#pragma once

// Source engine's 4-byte RGBA color
struct Color
{
    Color(unsigned char Red, unsigned char Green, unsigned char Blue, unsigned char Alpha = 255)
    {
        RGBA[0] = Red;
        RGBA[1] = Green;
        RGBA[2] = Blue;
        RGBA[3] = Alpha;
    }

    unsigned char RGBA[4];
};