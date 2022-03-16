/*
 * Orbital simulation
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "raylib-cpp.hpp"
#include "raymath.h"
#include "OrbitalBody.h"

#define NRO_ASTEROIDS 1000

class OrbitalSim
{
public:
	OrbitalSim(float timeStep); //OrbitalSim *makeOrbitalSim(float timeStep);
	~OrbitalSim(); //void freeOrbitalSim(OrbitalSim *sim);
    
    void updateOrbitalSim();
   
    float timeSinceStart;
    float quantity;
    OrbitalBody* ptr;

private:
    void acerlationCalc();
    void velocityCalc();
    void positionCalc();

    float timeStep;
};

#endif
