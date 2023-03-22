#pragma once
#include "Math/Vector.h"

class Color: public fw::Vector4
{

public:
    Color() : Vector4(0, 0, 0, 0) {}
    Color(Vector4 v) : Vector4(v) {}
    Color(float R, float G, float B, float A) : Vector4(R,G,B,A) {};
    Color(fw::Vector3 _color, float _alpha) : Vector4(_color, _alpha) {};

    static const Color White()   { return Color(1, 1, 1, 1); }
    static const Color Black()   { return Color(0, 0, 0, 1); }

    static const Color Red()     { return Color(1, 0, 0, 1); }
    static const Color Green()   { return Color(0, 1, 0, 1); }
    static const Color Blue()    { return Color(0, 0, 1, 1); }

    static const Color Cyan()    { return Color(0, 1, 1, 1); }
    static const Color Magenta() { return Color(1, 0, 1, 1); }
    static const Color Yellow()  { return Color(1, 1, 0, 1); }

    static const Color Orange()  { return Color(1, 0.4f, 0.07f, 1); }

    void SetAlpha(float _alpha) { w = _alpha; }
    void SetWithoutAlpha(Color _color) { x = _color.x;  y = _color.y; z = _color.z; }

};