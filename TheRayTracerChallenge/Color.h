#pragma once

#include "Constants.h"

class Color
{
public:
    float r, g, b;

    Color();
    Color(float r, float g, float b);
    ~Color() = default;
    
    //Operator overloads
    bool operator== (Color c);
    bool operator!= (Color c);
    Color operator+( Color c);
    Color operator- (Color c);
    Color operator* (float scalar);
    Color operator* (Color c);
};

inline Color::Color()
{
    r = g = b = 0.0f;
}

inline Color::Color(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

inline bool Color::operator==(Color c)
{
    return
        Constants::FloatEqual(r, c.r)
        && Constants::FloatEqual(g, c.g)
        && Constants::FloatEqual(b, c.b);
}

inline bool Color::operator!=(Color c)
{
    return !(*this == c);
}

inline Color Color::operator+(Color c)
{
    return Color(r + c.r, g + c.g, b + c.b);
}

inline Color Color::operator-(Color c)
{
    return Color(r - c.r, g - c.g, b - c.b);
}

inline Color Color::operator*(float scalar)
{
    return Color(
        r * scalar,
        g * scalar,
        b * scalar
    );
}

//Calculate the hadamard product
inline Color Color::operator*(Color c)
{
    return Color(
        r * c.r,
        g * c.g,
        b * c.b
    );
}
