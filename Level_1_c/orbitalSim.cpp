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
#include "ephemerides.h"
#include <iostream>

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F

// Prototypes
float getCenterMass(OrbitalSim* sim);
void acerlationCalc(OrbitalSim* sim);
void velocityCalc(OrbitalSim* sim);
void positionCalc(OrbitalSim* sim);
 
// Gets a random value between min and max
float getRandomFloat(float min, float max)
{
    return min + (max - min) * rand() / (float)RAND_MAX;
}

float getCenterMass(OrbitalSim* sim)
{
    int i;
    float maxMass=0;
    for (i=0;i<sim->quantity;i++)
    {
        OrbitalBody* ptrBody = sim->ptr + i;
        if (ptrBody->mass>maxMass)
        {
            maxMass = ptrBody->mass;
        }
    }
    return maxMass;
}
// Places an asteroid
//
// centerMass: mass of the most massive object in the star system
void placeAsteroid(OrbitalBody *body, float centerMass)
{
    // Logit distribution
    float x = getRandomFloat(0, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabs(l));
    float phi = getRandomFloat(0, 2 * M_PI);

    // Surprise!
    // phi = 0;

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    // Fill in with your own fields:
     body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
     body->size = 2E3F; // Typical asteroid radius: 2km
     body->color = GRAY;
     body->coordinates = {r * cosf(phi), 0, r * sinf(phi)};
     body->vel = {-v * sinf(phi), vy, v * cosf(phi)};
}

// Make an orbital simulation
OrbitalSim* makeOrbitalSim(float timeStep)
{
    int i;
    OrbitalSim* sim = (OrbitalSim*)malloc(sizeof(OrbitalSim));
    sim->quantity = SOLARSYSTEM_BODYNUM + NRO_ASTEROIDS;
    sim->timeSinceStart = 0;
    sim->timeStep = timeStep;
    sim->ptr = (OrbitalBody*)malloc(sim->quantity * sizeof(OrbitalBody));
    for (i = 0; i < SOLARSYSTEM_BODYNUM; i++)
    {
        OrbitalBody* ptrBody = sim->ptr + i ;
        ptrBody->color = solarSystem[i].color;
        ptrBody->vel = solarSystem[i].velocity;
        ptrBody->mass = solarSystem[i].mass;
        ptrBody->coordinates = solarSystem[i].position;
        ptrBody->size = solarSystem[i].radius;
    }

/*    for (i = 0; i < ALPHACENTAURISYSTEM_BODYNUM; i++)
    {
        OrbitalBody* ptrBody = sim->ptr + (i);
        ptrBody->color = alphaCentauriSystem[i].color;
        ptrBody->vel = alphaCentauriSystem[i].velocity;
        ptrBody->mass = alphaCentauriSystem[i].mass;
        ptrBody->coordinates = alphaCentauriSystem[i].position;
        ptrBody->size = alphaCentauriSystem[i].radius;
    }
*/
    float massiveMass = getCenterMass(sim);
    for (i = 0; i < NRO_ASTEROIDS; i++)
    {
        OrbitalBody* ptrBody = sim->ptr + (i+SOLARSYSTEM_BODYNUM);
        placeAsteroid(ptrBody, massiveMass);
    }
    return sim;
}

// Simulates a timestep
void updateOrbitalSim(OrbitalSim *sim)
{
    sim->timeSinceStart += sim->timeStep;
    acerlationCalc(sim);
    velocityCalc(sim);
    positionCalc(sim);
}

void freeOrbitalSim(OrbitalSim *sim)
{
    free(sim->ptr);
    free(sim);
}

void acerlationCalc(OrbitalSim* sim)
{
    int i, j;
    for (i = 0; i < sim->quantity; i++)
    {
        OrbitalBody* bodyi = sim->ptr + i;
        Vector3 aceleracionNeta = { 0,0,0 };
        for (j = 0; j < (sim->quantity - NRO_ASTEROIDS); j++)
        {
            if (i != j)
            {
                OrbitalBody* bodyj = sim->ptr + j;
                Vector3 resta = Vector3Subtract(bodyi->coordinates, bodyj->coordinates);
                float modulo = Vector3Length(resta);
                aceleracionNeta = Vector3Add(aceleracionNeta, Vector3Scale(resta, bodyj->mass /(modulo * modulo * modulo)));
            }
        }
        bodyi->aceleration = Vector3Scale(aceleracionNeta, -GRAVITATIONAL_CONSTANT);
    }
}

void velocityCalc(OrbitalSim* sim)
{
    int i;
    for (i = 0; i < sim->quantity; i++)
    {
        OrbitalBody* bodyi = sim->ptr + i;
        bodyi->vel = Vector3Add(bodyi->vel, Vector3Scale(bodyi->aceleration, sim->timeStep));
    }
}

void positionCalc(OrbitalSim* sim)
{
    int i;
    for (i = 0; i < sim->quantity; i++)
    {
        OrbitalBody* bodyi = sim->ptr + i;
        bodyi->coordinates = Vector3Add(bodyi->coordinates, Vector3Scale(bodyi->vel, sim->timeStep));
    }
}