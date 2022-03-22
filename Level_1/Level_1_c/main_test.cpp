
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
 * Tests
 */

#include <iostream>

#include "orbitalSim.h"

#define SECONDS_PER_DAY 86400.0F

using namespace std;

int main()
{
	float fps = 60.0F;                            // frames per second
	float timeMultiplier = 100 * SECONDS_PER_DAY; // Simulation speed: 100 days per real second
	float timeStep = timeMultiplier / fps;

	OrbitalSim* sim = makeOrbitalSim(timeStep);

	// Modify to suit your code:
	if (sim->ptr[0].coordinates.x != -1.283674643550172E+09F)
	{
		cout << "OrbitSim not initialized correctly" << endl;
		return 1;
	}

	updateOrbitalSim(sim);

	// Modify to suit your code:
	if (sim->ptr[0].coordinates.x != -1.284506496E+09F)
	{
		cout << "OrbitSim not updated correctly" << endl;
		return 2;
	}

	return 0;
}
