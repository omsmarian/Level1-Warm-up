/*
 * Orbital simulation view
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
 *
 * Grupo 6
 * Participants: Tomas Wickham, Mariano Oms
 *
 * Level 1a:
 *
 * This file is responsible of displaying the simulation
 */

#include <time.h>

#include "orbitalSimView.h"

#define SCALE_FACTOR 1E-11

 /* Calculates the date since the sim was started
 * Input: Time since the simultaion started
 * Output: Pointer to a sting of char
 */
const char* getISODate(float currentTime) {
	// Epoch: 2022-01-01
	struct tm epoch_tm = { 0, 0, 0, 1, 0, 122 };
	time_t epoch = mktime(&epoch_tm);

	// Convert epoch time to local time
	time_t local_time = epoch + (time_t)currentTime;

	// Print ISO date for local time
	struct tm* local_tm = localtime(&local_time);
	return TextFormat("Date: %04d-%02d-%02d", 1900 + local_tm->tm_year, local_tm->tm_mon + 1, local_tm->tm_mday);
}

/* Displays the simulation by drawing the planets and asteroids
* Input: Pointer to the simulation
*/
void renderOrbitalSim3D(OrbitalSim* sim)
{
	int i;
	for (i = 0; i < sim->quantity - NRO_ASTEROIDS; i++)
	{
		OrbitalBody* carry = sim->ptr + i;
		Vector3 origin = Vector3Scale(carry->coordinates, SCALE_FACTOR);
		float radius = 0.005F * logf(carry->size);
		DrawSphere(origin, radius, carry->color);
		DrawPoint3D(origin, carry->color);
	}
	for (i = 0; i < NRO_ASTEROIDS; i++)
	{
		OrbitalBody* carry = sim->ptr + (i + 9);
		Vector3 origin = Vector3Scale(carry->coordinates, SCALE_FACTOR);
		DrawPoint3D(origin, carry->color);
	}
}

/* Displays the FPS and the current date
* Input: Pointer to the simulation
*/
void renderOrbitalSim2D(OrbitalSim* sim)
{
	DrawFPS(0, 0);
	DrawText(getISODate(sim->timeSinceStart), 0, 25, 20, GREEN);
}
