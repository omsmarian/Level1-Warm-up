/*
 * Orbital simulation view
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
 */

#include <time.h>

#include "orbitalSimView.h"

#define SCALE_FACTOR 1E-11

const char* View::getISODate() {
    // Epoch: 2022-01-01
    struct tm epoch_tm = { 0, 0, 0, 1, 0, 122 };
    time_t epoch = mktime(&epoch_tm);

    // Convert epoch time to local time
    time_t local_time = epoch + (time_t)(this->sim->timeSinceStart);

    // Print ISO date for local time
    struct tm* local_tm = localtime(&local_time);
    return TextFormat("Date: %04d-%02d-%02d", 1900 + local_tm->tm_year, local_tm->tm_mon + 1, local_tm->tm_mday);
}
/*fun1
fun2

*sim

sim->...       this->sim.ptr +i */


void View::renderOrbitalSim3D()
{
    int i;
    for (i = 0; i < this->sim->quantity - NRO_ASTEROIDS ; i++)
    {
        OrbitalBody* carry = this->sim->ptr + i;
        Vector3 origin = Vector3Scale(carry->coordinates,SCALE_FACTOR);
        float radius = 0.005F * logf(carry->size);
        DrawSphere(origin, radius, carry->color);
        DrawPoint3D(origin, carry->color);
    }
    for (i = 0; i < NRO_ASTEROIDS; i++)
    {
        int quant = this->sim->quantity;
        OrbitalBody* carry = this->sim->ptr + i + quant- NRO_ASTEROIDS;
        Vector3 origin = Vector3Scale(carry->coordinates, SCALE_FACTOR);
        DrawPoint3D(origin, carry->color);
    }
}

void View::renderOrbitalSim2D()
{
    DrawFPS(0, 0);
    DrawText(getISODate(), 0, 25, 20, GREEN);
}

View::View(OrbitalSim* sim)
{
    this->sim = sim;
}