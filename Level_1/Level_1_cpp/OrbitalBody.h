/*
 * Orbital body
 *
 * 22.08 EDA
 *
 * Grupo 6
 * Participants: Tomas Wickham, Mariano Oms
 *
 * Level 1a:
 *
 * This file contains the body class
 */

#ifndef ORBITALBODY_H
#define ORBITALBODY_H

#include "raylib-cpp.hpp"
#include "raymath.h"

class OrbitalBody
{
public:
	OrbitalBody();

	void setColor(Color color);
	void setRadius(float size);
	void setMass(float mass);
	void setCoordinates(Vector3 coordinates);
	void setVelocity(Vector3 velocity);
	void setAceleration(Vector3 acleration);

	Color getColor();
	float getRadius();
	float getMass();
	Vector3 getCoordinates();
	Vector3 getVelocity();
	Vector3 getAceleration();

private:

	Vector3 aceleration;
	Vector3 vel;
	Vector3 coordinates;
	float mass;
	float size;
	Color color;

};

#endif