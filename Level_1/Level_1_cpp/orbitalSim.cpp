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
 * This file is responsible for inicializing, updating and realesing the simulation
 */

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>

#include "orbitalSim.h"
#include "OrbitalBody.h"
#include "ephemerides.h"

#define NRO_BODIS (SOLARSYSTEM_BODYNUM + NRO_ASTEROIDS)
#define ASTEROIDS_MEAN_RADIUS 4E11F
#define RAND_FLOAT(min,max) ((min) + ((max) - (min)) * rand() / (float)RAND_MAX)


 /* Creates the simulation
 * Input: The speed in which the simulation will run
 */
OrbitalSim::OrbitalSim(float timeStep)
{
	this->quantity = NRO_BODIS;
	this->timeSinceStart = 0;
	this->timeStep = timeStep;
	OrbitalBody* OrbitalBodys = new OrbitalBody[NRO_BODIS];		// generates an array of NRO_BODIS dimensions
	int i;
	float maxMass = 0;
	for (i = 0; i < SOLARSYSTEM_BODYNUM; i++) // goes through the planets defined in ephemerides.h
	{
		EphemeridesBody planet = solarSystem[i];
		OrbitalBodys[i].setColor(planet.color);
		OrbitalBodys[i].setVelocity(planet.velocity);
		OrbitalBodys[i].setMass(planet.mass);
		OrbitalBodys[i].setRadius(planet.radius);
		OrbitalBodys[i].setCoordinates(planet.position);

		if (OrbitalBodys[i].getMass() > maxMass)		// Compares and saves the mass of the largest body
		{
			maxMass = OrbitalBodys[i].getMass();
		}
	}
	for (i = SOLARSYSTEM_BODYNUM; i < NRO_BODIS; i++) // goes through the asteroids defined
	{
		// Logit distribution
		float x = RAND_FLOAT(0, 1);
		float l = logf(x) - logf(1 - x) + 1;

		// https://mathworld.wolfram.com/DiskPointPicking.html
		float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabs(l));
		float phi = RAND_FLOAT(0, 2 * M_PI);

		// https://en.wikipedia.org/wiki/Circular_orbit#Velocity
		float v = sqrtf(GRAVITATIONAL_CONSTANT * maxMass / r) * RAND_FLOAT(0.6F, 1.2F);
		float vy = RAND_FLOAT(-1E2F, 1E2F);

		// Charactreristics of the asteroids
		OrbitalBodys[i].setMass(1E12F);		// Typical asteroid weight: 1 billion tons
		OrbitalBodys[i].setRadius(2E3F);	// Typical asteroid radius: 2km
		OrbitalBodys[i].setColor(GRAY);
		OrbitalBodys[i].setCoordinates({ r * cosf(phi), 0, r * sinf(phi) });
		OrbitalBodys[i].setVelocity({ -v * sinf(phi), vy, v * cosf(phi) });
	}
	this->ptr = OrbitalBodys;
}

// Releases the simulation
OrbitalSim::~OrbitalSim()
{
	delete[] this->ptr;
}

// Simulates a timestep, and updates the object trajectory
void OrbitalSim::updateOrbitalSim()
{
	this->timeSinceStart += this->timeStep;
	acerlationCalc();
	velocityCalc();
	positionCalc();
}

// Calculates the aceleration of every object
void OrbitalSim::acerlationCalc()
{
	int i, j;
	for (i = 0; i < this->quantity; i++)
	{
		OrbitalBody* bodyi = this->ptr + i;
		Vector3 aceleracionNeta = { 0,0,0 };
		for (j = 0; j < (this->quantity - NRO_ASTEROIDS); j++)
		{
			if (i != j)
			{
				OrbitalBody* bodyj = this->ptr + j;
				Vector3 resta = Vector3Subtract(bodyi->getCoordinates(), bodyj->getCoordinates());
				float modulo = Vector3Length(resta);						// Calculus based on "Physics brushup" on level1 information
				aceleracionNeta = Vector3Add(aceleracionNeta, Vector3Scale(resta, bodyj->getMass() / (modulo * modulo * modulo)));
			}
		}
		bodyi->setAceleration(Vector3Scale(aceleracionNeta, -GRAVITATIONAL_CONSTANT));
	}
}

// Calculates the velocity of every object
void OrbitalSim::velocityCalc()
{
	int i;
	for (i = 0; i < this->quantity; i++)
	{
		OrbitalBody* bodyi = this->ptr + i;
		bodyi->setVelocity(Vector3Add(bodyi->getVelocity(), Vector3Scale(bodyi->getAceleration(), this->timeStep)));
	}
}

// Calculates the coordinates of every object
void OrbitalSim::positionCalc()
{
	int i;
	for (i = 0; i < this->quantity; i++)
	{
		OrbitalBody* bodyi = this->ptr + i;
		bodyi->setCoordinates(Vector3Add(bodyi->getCoordinates(), Vector3Scale(bodyi->getVelocity(), this->timeStep)));
	}
}

float OrbitalSim::getTimeSinceStart()
{
	return this->timeSinceStart;
}

float OrbitalSim::getQuantity()
{
	return this->quantity;
}

OrbitalBody* OrbitalSim::getPtr()
{
	return this->ptr;
}