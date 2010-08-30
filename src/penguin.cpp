//  The Flying Penguin
//  Copyright (C) 2010 Lee Zher Huei <lee@leezh.net>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//  MA 02110-1301, USA.
//

#include "main.hpp"

namespace penguin {

vect position;
vect velocity;
float angle;
bool thrusters;
float fuelRemaining;

bool running;
bool takeoff;
bool stall;
bool underspeed;
float airSpeed;

// Note that everything is using SI units. That also means angles are
// in radians.

// These values are tweaked over several play-tests and is designed
// such that it is still reasonable for those who do not conservere
// fuel (Average ~1000m)
const float stallAngle = rad(20);
const float wingAngle = rad(10);
const float wingArea = 3;
const float mass = 50;
const float dragConst = 0.75;
const float thrustForce = 9.81 * 0.9;
const float restitution = 0.2;
const float takeoffHeight = 2;
const float underspeedVel = 12; // Obtained experimentally

// References to find the lift coefficient below stall point.
// http://www.grc.nasa.gov/WWW/K-12/airplane/incline.html
//
// c = 2 * pi * angle(rad)
// c = 6.28318530718 * angle(rad)
inline float liftCoef(float a) {
    if (a < stallAngle && a > 0) {
        stall = false;
        return 2 * PI * a;
    } else {
        stall = true;
        return 0;
    }
}

// References to find lift as a function of angle, velocity and
// wing area
// http://wright.nasa.gov/airplane/lifteq.html
//
// lift = coefficient * density * vel^2 * wing area / 2
// By the way, calculating vel^2 is slower than vel * vel
// Also, a here is angle of wing relative to windspeed
inline float lift(float a, float vel) {
    airSpeed = vel;
    underspeed = (vel < underspeedVel);
    return liftCoef(a) * 1.293 * vel * vel * wingArea / 2;
}

void doPhysics(float deltaTime) {
    if (angle > PI) angle -= 2 * PI;
    if (angle < -PI) angle += 2 * PI;

    if (!running) return;

    // Gravity
    vect acceleration(0, -9.81);

    // Drag
    acceleration -= velocity * dragConst * velocity.magnitude() / mass;

    // Thrusters
    if (thrusters && fuelRemaining > 0) {
        float boostForce = thrustForce;
        fuelRemaining -= deltaTime;
        acceleration += vect(boostForce * cos(angle), boostForce * sin(angle));
    }
    if (fuelRemaining <= 0) {
        fuelRemaining = 0;
    }

    // Lift
    if (velocity.magnitude() != 0) {
        float angleOfAttack = atan2(velocity.j, velocity.i) - angle + wingAngle;
        float liftForce = lift(angleOfAttack, velocity.magnitude() * cos(angleOfAttack));
        acceleration += vect(-liftForce * sin(angle), liftForce * cos(angle)) / mass;
    }

    velocity += (acceleration * deltaTime);
    position += (velocity * deltaTime);
    // If we're crashing into the ground
    if (position.j < 0) {
        position.j = 0;

        if (velocity.j < 0) {
            velocity.j = -velocity.j * restitution;
        }

        // Brakes
        if (!thrusters) {
            if (velocity.i > 1 * deltaTime) {
                velocity.i -= 1 * deltaTime;
            } else if (velocity.i < -1 * deltaTime) {
                velocity.i += 1 * deltaTime;
            } else {
                velocity.i = 0;
            }
        }

        if (takeoff && velocity.magnitude() < 0.2) {
                running = false;
                return;
        }

        if (angle < 0) angle += 0.8 * deltaTime;
        if (angle > 0) angle -= 0.8 * deltaTime;
    }

    if (position.j > takeoffHeight) {
        takeoff = true;
    }

    if (!takeoff) {
        // Limit the angle on the ground
        if (angle > rad(2)) angle = rad(2);
        if (angle < rad(-20)) angle = rad(-20);

        // Also, you can't stall on the ground :)
        stall = false;
        underspeed = false;
    }
}

void reset() {
    position = vect(0, 0);
    velocity = vect(0, 0);
    angle = 0;
    thrusters = false;
    fuelRemaining = 15;

    running = true;
    takeoff = false;
    stall = false;
    underspeed = false;
}
}
