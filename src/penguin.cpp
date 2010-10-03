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

Penguin penguin;

// In the future, I plan to add an in-game console to modify the values
// without recompiling.

// Physics related variables
float stallAngle = rad(20); // Angle when the wings stall
float wingAngle = rad(10); // Angle the wings make relative to body
float gravityAccel = 9.81f; // Acceleration due to free-fall
float thrustAccel = 3.f; // Acceleration component of thrust
float brakeAccel = 1.f; // Brakeing deceleration when on the ground
float liftConst = 0.19f; // 2 * pi * air density *  wing area / mass
float dragConst = 0.005f; // 0.5 * drag coefficient * air density * reference area / mass
float restitution = 0.2f; // Restitution coefficient when bouncing
float turningConst = 3.f; // 1 / Rotational damping

float startingFuel = 15.f; // Seconds of fuel given initially
float underspeedVel = 10.f; // When to give the warning that lift is low
float takeoffHeight = 2.f; // Height at which to hide the instructions
float stopSpeed = 0.1f; // Solves the rounding errors when slowing down

bool infiniteFuel = true; // DEBUG: Infinite fuel
bool invincible = false; // DEBUG: Never die

bool Penguin::isAlive() {return running;}
bool Penguin::isFlying() {return takeoff;}

bool Penguin::isStalling() {
    if (windAngle() > stallAngle || windAngle() < -stallAngle) {
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
float Penguin::liftAccel(float angle) {
    if (angle > stallAngle || angle < -stallAngle) {
        float speed = windSpeed();
        return speed * speed * liftConst * (angle);
    }
    return 0;
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
    float speed = windSpeed();
    
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
        float lift = speed * speed * liftConst * (windAngle() + wingAngle + elevatorAngle);
        accel += vect(0, 0, 1).crossProduct(vel).unitVector() * lift;
        //accel += vect(-lift * sin(angle), lift * cos(angle));
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
    
    if (vel.magnitude() > 0.f) {
        float vAngle = atan2(vel.y, vel.x);
        angularVel = 0;
        if (abs(angle - vAngle) < PI) {
            if (angle > vAngle) {
                angularVel -= turningConst * speed * deltaTime;
            } else {
                angularVel += turningConst * speed * deltaTime;
            }
        } else {
            if (angle > vAngle) {
                angularVel += turningConst * speed * deltaTime;
            } else {
                angularVel -= turningConst * speed * deltaTime;
            }
        }
        angularVel += turningConst * speed * speed * liftConst * elevatorAngle * deltaTime;
        angle += angularVel * deltaTime;
    }

    if(pos.y > takeoffHeight) {
        takeoff = true;
    }
}
void Penguin::init() {
    sprite.SetImage(res::img("penguin"));
    sprite.SetCenter(res::img("penguin").GetWidth() * 0.5f, res::img("penguin").GetHeight() * 0.5f);
}
void Penguin::render() {
    sprite.Resize(scale * 3, scale * 3);
    sprite.SetPosition(res::window.GetWidth() / 2.f, res::window.GetHeight() / 2.f);
    sprite.SetRotation(deg(angle));
    
    res::window.Draw(sprite);
}
