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
 * This file contains the class responsible for displaying the simulation
 */

#ifndef ORBITALSIMVIEW_H
#define ORBITALSIMVIEW_H

#include "orbitalSim.h"
#include "OrbitalBody.h"

class View
{
public:
	View(OrbitalSim* sim);
	void renderOrbitalSim3D();
	void renderOrbitalSim2D();
private:
	const char* getISODate();
	OrbitalSim* sim;
};



#endif
