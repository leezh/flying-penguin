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

#include <SFML/Graphics.hpp>
#include "main.hpp"
#include "resources.hpp"

Penguin mpenguin;

// In the future, I plan to add an in-game console to modify the values
// without recompiling.

// Physics related variables
float stallAngle = rad(20); // Angle when the wings stall
float wingAngle = rad(10); // Angle the wings make relative to body
float gravityAccel = 9.81; // Acceleration due to free-fall
float thrustAccel = 7; // Acceleration component of thrust
float brakeAccel = 1; // Braking deceleration when on the ground
float liftConst = 0.19; // 2 * pi * air density *  wing area
float dragConst = 0.015; // 0.5 * drag coefficient * air density * reference area
float restitution = 0.2; // Restitution coefficient when bouncing
float turningConst = 2; // 1 / Rotational damping

float startingFuel = 15; // Seconds of fuel given initially
float underspeedVel = 10; // When to give the warning that lift is low
float takeoffHeight = 2; // Height at which to hide the instructions
float stopSpeed = 0.1; // Solves the rounding errors when slowing down
float speedToTurn = 2; // Solves the jerkiness when lift-off

bool infiniteFuel = true; // DEBUG: Infinite fuel
bool invincible = true; // DEBUG: Never die

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

Penguin::Penguin() {
    sprite.SetImage(res::img("penguin"));
    sprite.SetCenter(res::img("penguin").GetWidth() * 0.5, res::img("penguin").GetHeight() * 0.5);
    sprite.Resize(scale * 3, scale * 3);
}

bool Penguin::isAlive() {return running;}
bool Penguin::isFlying() {return pos.y > takeoffHeight;}

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
    return angle - atan2(vel.y, vel.x);
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
    if (pos.y <= 0) {
        pos.y = 0;
        
        // Bounce
        if (vel.y < 0) {
            vel.y = -vel.y * restitution;
        }
        
        // Brakes
        if (!(thrust && fuel > 0)) {
            float deltaVel = brakeAccel * deltaTime;
            if (vel.x > deltaVel) {
                vel.x -= deltaVel;
            } else if (vel.x < -deltaVel) {
                vel.x += deltaVel;
            } else {
                vel.x = 0;
            }
            
            // Seems like it has stopped
            if (takeoff && vel.magnitude() < stopSpeed && !invincible) {
                running = false;
                return;
            }
        }
    }
    
    if (isFlying() || vel.magnitude() > speedToTurn) {
        float vAngle = atan2(vel.y, vel.x);
        if (abs(angle - vAngle) < turningConst * deltaTime) {
            angle = vAngle;
        }
        if (abs(angle - vAngle) < PI) {
            if (angle > vAngle) {
                angle -= turningConst * deltaTime;
            } else {
                angle += turningConst * deltaTime;
            }
        } else {
            if (angle > vAngle) {
                angle += turningConst * deltaTime;
            } else {
                angle -= turningConst * deltaTime;
            }
        }
    }
}

void Penguin::render() {
    sprite.SetPosition(res::window.GetWidth() / 2, res::window.GetHeight() / 2);
    sprite.SetRotation(deg(angle));
    
    res::window.Draw(sprite);
}
