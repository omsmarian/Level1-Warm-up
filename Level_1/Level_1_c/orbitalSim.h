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
 * This file containts the stucturs of the simulation
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H


#include "raylib.h"
#include "raymath.h"

#define NRO_ASTEROIDS 1000

typedef struct
{
	Vector3 vel;
	Vector3 coordinates;
	Vector3 aceleration;
	float mass;
	float size;
	Color color;
} OrbitalBody;

typedef struct
{
	float timeStep;
	float timeSinceStart;
	float quantity;
	OrbitalBody* ptr;
}OrbitalSim;

OrbitalSim* makeOrbitalSim(float timeStep);
void updateOrbitalSim(OrbitalSim* sim);
void freeOrbitalSim(OrbitalSim* sim);

#endif
