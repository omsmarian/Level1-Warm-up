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
 * Output: Pointer to a sting of char
 */
const char* View::getISODate() {
	// Epoch: 2022-01-01
	struct tm epoch_tm = { 0, 0, 0, 1, 0, 122 };
	time_t epoch = mktime(&epoch_tm);

	// Convert epoch time to local time
	time_t local_time = epoch + (time_t)(this->sim->getTimeSinceStart());

	// Print ISO date for local time
	struct tm* local_tm = localtime(&local_time);
	return TextFormat("Date: %04d-%02d-%02d", 1900 + local_tm->tm_year, local_tm->tm_mon + 1, local_tm->tm_mday);
}

// Displays the simulation by drawing the planets and asteroids
void View::renderOrbitalSim3D()
{
	int i;
	for (i = 0; i < this->sim->getQuantity() - NRO_ASTEROIDS; i++)      // goes through the number of planets
	{
		OrbitalBody* carry = this->sim->getPtr() + i;
		Vector3 origin = Vector3Scale(carry->getCoordinates(), SCALE_FACTOR);
		float radius = 0.005F * logf(carry->getRadius());
		DrawSphere(origin, radius, carry->getColor());
		DrawPoint3D(origin, carry->getColor());
	}
	for (i = 0; i < NRO_ASTEROIDS; i++)             // goes through the number of asteroids
	{
		int quant = this->sim->getQuantity();
		OrbitalBody* carry = this->sim->getPtr() + i + quant - NRO_ASTEROIDS;
		Vector3 origin = Vector3Scale(carry->getCoordinates(), SCALE_FACTOR);
		DrawPoint3D(origin, carry->getColor());
	}
}

// Displays the FPS and the current date
void View::renderOrbitalSim2D()
{
	DrawFPS(0, 0);
	DrawText(getISODate(), 0, 25, 20, GREEN);
}

/* Creates a copy of the memory location for later use
* Input: Pointer to the simulation
*/
View::View(OrbitalSim* sim)
{
	this->sim = sim;
}