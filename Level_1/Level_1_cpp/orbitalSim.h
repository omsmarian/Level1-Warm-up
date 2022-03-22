/*
 * Orbital simulation
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
 *
 * Grupo 6
 * Participants: Tomas Wickham, Mariano Oms
 *
 * Level 1a:
 *
 * This file contains the simulation class
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
	OrbitalSim(float timeStep);
	~OrbitalSim();

	void updateOrbitalSim();
	float getTimeSinceStart();
	float getQuantity();
	OrbitalBody* getPtr();

private:
	void acerlationCalc();
	void velocityCalc();
	void positionCalc();

	float timeSinceStart;
	float quantity;
	float timeStep;
	OrbitalBody* ptr;
};

#endif
