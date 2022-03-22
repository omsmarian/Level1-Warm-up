/*
 * Orbital simulation
 *
 * 22.08 EDA
 *
 * Grupo 6
 * Participants: Tomas Wickham, Mariano Oms
 *
 * Level 1a:
 *
 * This file initializes the bodis with zeros
 */

#define _USE_MATH_DEFINES

#include <math.h>

#include "OrbitalBody.h"

OrbitalBody::OrbitalBody()
{
	this->mass = 0;
	this->size = 0;
	this->color = BLACK;
	this->coordinates = { 0,0,0 };
	this->vel = { 0,0,0 };
	this->aceleration = { 0,0,0 };
}

void OrbitalBody::setColor(Color color)
{
	this->color = color;
}

void OrbitalBody::setRadius(float size)
{
	this->size = size;
}

void OrbitalBody::setMass(float mass)
{
	this->mass = mass;
}

void OrbitalBody::setCoordinates(Vector3 coordinates)
{
	this->coordinates = coordinates;
}

void OrbitalBody::setVelocity(Vector3 velocity)
{
	this->vel = velocity;
}

void OrbitalBody::setAceleration(Vector3 aceleration)
{
	this->aceleration = aceleration;
}

Color OrbitalBody::getColor()
{
	return this->color;
}

float OrbitalBody::getMass()
{
	return this->mass;
}

float OrbitalBody::getRadius()
{
	return this->size;
}

Vector3 OrbitalBody::getCoordinates()
{
	return this->coordinates;
}

Vector3 OrbitalBody::getVelocity()
{
	return this->vel;
}

Vector3 OrbitalBody::getAceleration()
{
	return this->aceleration;
}