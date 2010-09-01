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

Penguin mpenguin;

// In the future, I plan to add an in-game console to modify the values
// without recompiling.

float stallAngle = rad(20);
float wingAngle = rad(10);
float gravityAccel = 9.81;
float thrustAccel = 7;
float brakeAccel = 1;
float liftConst = 0.19;
float dragConst = 0.015;
float restitution = 0.2;

float startingFuel = 15;
float underspeedVel = 10;
float takeoffHeight = 2;
float stopSpeed = 0.3;

bool infiniteFuel = true;
bool invincible = true;

namespace penguin {

vect position;
vect velocity;
float angle;
bool thrusters;
float fuelRemaining;

bool running;
bool takeoff;

void doPhysics(float deltaTime) {
    mpenguin.thrust = thrusters;
    mpenguin.fuel = fuelRemaining;
    
    mpenguin.doPhysics(deltaTime);
    position = mpenguin.pos;
    velocity = mpenguin.vel;
    angle = mpenguin.angle;
    fuelRemaining = mpenguin.fuel;
    running = mpenguin.isAlive();
    takeoff = mpenguin.isFlying();
}

void reset() {
    mpenguin.reset();
    fuelRemaining = startingFuel;
    angle = 0;
}
}


bool Penguin::isAlive() {return running;}
bool Penguin::isFlying() {return takeoff;}

bool Penguin::isStalling() {
    if (windAngle() > stallAngle || windAngle() < -wingAngle) {
        return true;
    }
    return false;
}
bool Penguin::isUnderspeed() {
    if (vel.magnitude() < underspeedVel) {
        return true;
    }
    return false;
}

float Penguin::windAngle() {
    return angle - atan2(vel.j, vel.i);
}
float Penguin::windSpeed() {
    return vel.magnitude() * cos(windAngle());
}

void Penguin::reset() {
    pos = vect(0, 0);
    vel = vect(0, 0);
    angle = 0;
    running = true;
    takeoff = false;
    thrust = false;
    fuel = startingFuel;
    elevatorAngle = 0;
}

void Penguin::doPhysics(float deltaTime) {
    if (angle > PI) angle -= 2 * PI;
    if (angle < -PI) angle += 2 * PI;
    
    if (!running) return;
    vect accel;
    
    // Gravity
    accel += vect(0, -gravityAccel);
    
    // Drag
    accel -= vel * vel.magnitude() * dragConst;
    
    // Thrusters
    if (thrust && fuel > 0) {
        accel += vect(thrustAccel * cos(angle), thrustAccel * sin(angle));
        if (!infiniteFuel) {
            fuel -= deltaTime;
        }
    }
    if (fuel < 0) {
        fuel = 0;
    }
    
    // Lift
    if (!isStalling()) {
        float speed = windSpeed();
        float lift = speed * speed * liftConst * (windAngle() + wingAngle + elevatorAngle);
        accel += vect(-lift * sin(angle), lift * cos(angle));
    }
    
    vel += (accel * deltaTime);
    pos += (vel * deltaTime);
    
    // Collision with the ground
    if (pos.j <= 0) {
        pos.j = 0;
        
        // Bounce
        if (vel.j < 0) {
            vel.j = -vel.j * restitution;
        }
        
        // Brakes
        if (!(thrust && fuel > 0)) {
            float deltaVel = brakeAccel * deltaTime;
            if (vel.i > deltaVel) {
                vel.i -= deltaVel;
            } else if (vel.i < -deltaVel) {
                vel.i += deltaVel;
            } else {
                vel.i = 0;
            }
            
            // Seems like it has stopped
            if (takeoff && vel.magnitude() < stopSpeed && !invincible) {
                running = false;
                return;
            }
        }
    }
    
    if (pos.j > takeoffHeight) {
        takeoff = true;
    }
    
    if (vel.magnitude() > stopSpeed) {
        float vAngle = atan2(vel.j, vel.i);
        if (abs(angle - vAngle) < PI) {
            if (angle > vAngle) {
                angle -= 0.5 * deltaTime;
            } else {
                angle += 0.5 * deltaTime;
            }
        } else {
            if (angle > vAngle) {
                angle += 0.5 * deltaTime;
            } else {
                angle -= 0.5 * deltaTime;
            }
        }
    }
}
