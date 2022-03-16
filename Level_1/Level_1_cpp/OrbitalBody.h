#ifndef ORBITALBODY_H
#define ORBITALBODY_H

//#include "raylib.h"
#include "raylib-cpp.hpp"
#include "raymath.h"

class OrbitalBody
{
public:
    OrbitalBody();
    ~OrbitalBody();

    Vector3 vel;
    Vector3 coordinates;
    Vector3 aceleration;
    float mass;
    float size;
    Color color;
};

#endif