/*
 * Orbital simulation
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
 */

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>

#include "orbitalSim.h"
#include "OrbitalBody.h"
#include "ephemerides.h"

#define NRO_BODYS (SOLARSYSTEM_BODYNUM + NRO_ASTEROIDS)
#define ASTEROIDS_MEAN_RADIUS 4E11F
#define RAND_FLOAT(min,max) ((min) + ((max) - (min)) * rand() / (float)RAND_MAX)


// Make an orbital simulation
OrbitalSim::OrbitalSim(float timeStep)
{
    this->quantity = NRO_BODYS;
    this->timeSinceStart = 0;
    this->timeStep = timeStep;
    OrbitalBody* OrbitalBodys= new OrbitalBody[NRO_BODYS];
    int i;
    float maxMass = 0;
    for (i=0;i<SOLARSYSTEM_BODYNUM;i++) //recorre los cuerpos celestes definidos en ephemerides
    {
        EphemeridesBody planet = solarSystem[i];
        OrbitalBodys[i].color = planet.color;
        OrbitalBodys[i].vel = planet.velocity;
        OrbitalBodys[i].mass = planet.mass;
        OrbitalBodys[i].size = planet.radius;
        OrbitalBodys[i].coordinates = planet.position;
        if (OrbitalBodys[i].mass > maxMass)
        {
            maxMass = OrbitalBodys[i].mass;
        }
    }
    for (i = SOLARSYSTEM_BODYNUM; i < NRO_BODYS; i++) //recorre los asteroides
    {
        // Logit distribution
        float x = RAND_FLOAT(0, 1);
        float l = logf(x) - logf(1 - x) + 1;

        // https://mathworld.wolfram.com/DiskPointPicking.html
        float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabs(l));
        float phi = RAND_FLOAT(0, 2 * M_PI);

        // Surprise!
        // phi = 0;

        // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
        float v = sqrtf(GRAVITATIONAL_CONSTANT * maxMass / r) * RAND_FLOAT(0.6F, 1.2F);
        float vy = RAND_FLOAT(-1E2F, 1E2F);

        // Fill in with your own fields:
        OrbitalBodys[i].mass = 1E12F;  // Typical asteroid weight: 1 billion tons
        OrbitalBodys[i].size = 2E3F; // Typical asteroid radius: 2km
        OrbitalBodys[i].color = GRAY;
        OrbitalBodys[i].coordinates = { r * cosf(phi), 0, r * sinf(phi) };
        OrbitalBodys[i].vel = { -v * sinf(phi), vy, v * cosf(phi) };
    }
    this->ptr = OrbitalBodys;
}


OrbitalSim::~OrbitalSim()
{
    delete[] this->ptr;
}

// Simulates a timestep
void OrbitalSim::updateOrbitalSim()
{
    this->timeSinceStart += this->timeStep;
    acerlationCalc();
    velocityCalc();
    positionCalc();
}

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
                Vector3 resta = Vector3Subtract(bodyi->coordinates, bodyj->coordinates);
                float modulo = Vector3Length(resta);
                aceleracionNeta = Vector3Add(aceleracionNeta, Vector3Scale(resta, bodyj->mass /(modulo * modulo * modulo)));
            }
        }
        bodyi->aceleration = Vector3Scale(aceleracionNeta, -GRAVITATIONAL_CONSTANT);
    }
}

void OrbitalSim::velocityCalc()
{
    int i;
    for (i = 0; i < this->quantity; i++)
    {
        OrbitalBody* bodyi = this->ptr + i;
        bodyi->vel = Vector3Add(bodyi->vel, Vector3Scale(bodyi->aceleration, this->timeStep));
    }
}

void OrbitalSim::positionCalc()
{
    int i;
    for (i = 0; i < this->quantity; i++)
    {
        OrbitalBody* bodyi = this->ptr + i;
        bodyi->coordinates = Vector3Add(bodyi->coordinates, Vector3Scale(bodyi->vel, this->timeStep));
    }
}