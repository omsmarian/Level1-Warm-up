/*
 * Orbital simulation view
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
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
