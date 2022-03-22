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

 /*
 * ¿Por qué se eligió usar Float para representar las masas, distancias, velocidades y aceleraciones?
 * En cuanto a esto se puede decir que el mayor numero representante de estos valores está determinado por las
 * distancias en metros, considerando que el sistema solar mide  12 * 10^12 metros, y el float va de -3.4 * 10^38 a +3.4 * 10^38,
 * por lo que puede manipular estos valores fácilmente. Además, en cuanto a la precisión por los decimales
 * en la mantissa se puede decir que una pequeña variación de esa medida en torno a la simulación de un sistema solar no es significativa.
 *
 * La complejidad numérica de la simulación es O(n^2), debido a que por cada cuerpo debe analizar su relación en el espacio junto al resto.
 * El valor de n es la cantidad de cuerpos celestes, siendo los asteroides los de mayor cantidad,
 * por lo que al ser significativamente menores en masa, no se los consideró en los cálculos de la simulación,
 * solucionando el cuello de botella en la parte de complejidad.
 *
 * El otro cambio realizado para solucionar cuellos de botella fue representar a los asteroides como puntos en el espacio,
 * en lugar de como esferas, de esta forma se ahorran bastantes recursos gráficos.
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
void acerlationCalc(OrbitalSim* sim);
void velocityCalc(OrbitalSim* sim);
void positionCalc(OrbitalSim* sim);

// Gets a random value between min and max
float getRandomFloat(float min, float max)
{
	return min + (max - min) * rand() / (float)RAND_MAX;
}

/* Finds the most massive object
* Imput: poniter to simulation, to go through the number of planets
* Output: Value of the greatest mass
*/
float getCenterMass(OrbitalSim* sim)
{
	int i;
	float maxMass = 0;
	for (i = 0; i < sim->quantity; i++)
	{
		OrbitalBody* ptrBody = sim->ptr + i;
		if (ptrBody->mass > maxMass)
		{
			maxMass = ptrBody->mass;
		}
	}
	return maxMass;
}
// Places an asteroid
//
// centerMass: mass of the most massive object in the star system
void placeAsteroid(OrbitalBody* body, float centerMass)
{
	// Logit distribution
	float x = getRandomFloat(0, 1);
	float l = logf(x) - logf(1 - x) + 1;

	// https://mathworld.wolfram.com/DiskPointPicking.html
	float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabs(l));
	float phi = getRandomFloat(0, 2 * M_PI);

	// https://en.wikipedia.org/wiki/Circular_orbit#Velocity
	float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
	float vy = getRandomFloat(-1E2F, 1E2F);

	body->mass = 1E12F;		// Typical asteroid weight: 1 billion tons
	body->size = 2E3F;		// Typical asteroid radius: 2km
	body->color = GRAY;
	body->coordinates = { r * cosf(phi), 0, r * sinf(phi) };
	body->vel = { -v * sinf(phi), vy, v * cosf(phi) };
}

/* Make an orbital simulation
* Imput: The speed in which the sim will update
* Output: Reference to the created simulation
*/
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
		OrbitalBody* ptrBody = sim->ptr + i;
		ptrBody->color = solarSystem[i].color;
		ptrBody->vel = solarSystem[i].velocity;
		ptrBody->mass = solarSystem[i].mass;
		ptrBody->coordinates = solarSystem[i].position;
		ptrBody->size = solarSystem[i].radius;
	}

	float massiveMass = getCenterMass(sim);
	for (i = 0; i < NRO_ASTEROIDS; i++)
	{
		OrbitalBody* ptrBody = sim->ptr + (i + SOLARSYSTEM_BODYNUM);
		placeAsteroid(ptrBody, massiveMass);
	}
	return sim;
}

/* Simulates a timestep, and updates the object trajectory
* Imput: poniter to simulation, to update the time since the simulation started
*/
void updateOrbitalSim(OrbitalSim* sim)
{
	sim->timeSinceStart += sim->timeStep;
	acerlationCalc(sim);
	velocityCalc(sim);
	positionCalc(sim);
}

/* Releases the memory used by the simultaion
* Imput: poniter to simulation
*/
void freeOrbitalSim(OrbitalSim* sim)
{
	free(sim->ptr);
	free(sim);
}

/* Calculates the aceleration of every object
* Imput: poniter to simulation, to go through the number
* of planets and their information
*/
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
				float modulo = Vector3Length(resta);            // Calculus based on "Physics brushup" on level1 information
				aceleracionNeta = Vector3Add(aceleracionNeta, Vector3Scale(resta, bodyj->mass / (modulo * modulo * modulo)));
			}
		}
		bodyi->aceleration = Vector3Scale(aceleracionNeta, -GRAVITATIONAL_CONSTANT);
	}
}

/* Calculates the velocity of every object
* Imput: poniter to simulation, to go through the number
* of planets and their information
*/
void velocityCalc(OrbitalSim* sim)
{
	int i;
	for (i = 0; i < sim->quantity; i++)
	{
		OrbitalBody* bodyi = sim->ptr + i;      //calculus based on "Physics brushup" on level1 information
		bodyi->vel = Vector3Add(bodyi->vel, Vector3Scale(bodyi->aceleration, sim->timeStep));
	}
}

/* Calculates the coordinates of every object
* Imput: poniter to simulation, to go through the number
* of planets and their information
*/
void positionCalc(OrbitalSim* sim)
{
	int i;
	for (i = 0; i < sim->quantity; i++)
	{
		OrbitalBody* bodyi = sim->ptr + i;      //calculus based on "Physics brushup" on level1 information
		bodyi->coordinates = Vector3Add(bodyi->coordinates, Vector3Scale(bodyi->vel, sim->timeStep));
	}
}