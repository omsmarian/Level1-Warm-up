#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>

#include "OrbitalBody.h"

OrbitalBody::OrbitalBody()
{
    this->mass = 0;  // Typical asteroid weight: 1 billion tons
    this->size = 0; // Typical asteroid radius: 2km
    this->color = BLACK;
    this->coordinates = {0,0,0};
    this->vel = {0,0,0};
    this->aceleration = { 0,0,0 };
}

OrbitalBody::~OrbitalBody()
{

}

