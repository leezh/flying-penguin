//  The Flying Penguin
//  Copyright (C) 2010-2011 Lee Zher Huei <lee.zh.92@gmail.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
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
#include "penguin.hpp"
#include "particles.hpp"
#include "record.hpp"
#include "world.hpp"

using namespace util;

bool Penguin::isAlive() {return running;}
bool Penguin::isFlying() {return takeoff;}

bool Penguin::isStalling(float angle) {
    confVar(float, stallAngle);
    
    if (angle > rad(stallAngle) || angle < -rad(stallAngle)) {
        return true;
    }
    return false;
}

bool Penguin::isUnderspeed() {
    confVar(float, underspeedVel);
    
    if (vel.magnitude() < underspeedVel) {
        return true;
    }
    return false;
}

float Penguin::windAngle() {
    if (vel.magnitude() > 0.f) {
        return acuteAngleDiff(angle, atan2(vel.y, vel.x));
    } else {
        return 0.f;
    }
}

float Penguin::windSpeed() {
    return vel.magnitude() * cos(windAngle());
}

float Penguin::liftAccel(float angle) {
    if (!isStalling(angle)) {
        confVar(float, liftConst);
        
        float speed = windSpeed();
        return speed * speed * liftConst * angle;
    }
    return 0;
}

void Penguin::doPhysics(float deltaTime) {
    while (angle > PI) angle -= 2 * PI;
    while (angle < -PI) angle += 2 * PI;
    
    if (!running) return;
    Vect accel;
    float speed = windSpeed();
    confVar(float, underspeedVel);
    confVar(float, gravityAccel);
    confVar(float, dragConst);
    confVar(float, thrustAccel);
    confVar(bool, infiniteFuel);
    confVar(float, wingAngle);
    confVar(float, turningConst);
    confVar(float, takeoffHeight);
    confVar(bool, invincible);
    confVar(float, starCreateDelay);
    
    // Gravity
    accel += Vect(0, -gravityAccel);
    
    // Drag
    accel -= vel * vel.magnitude() * dragConst;
    
    // Thrusters
    if (thrust && (infiniteFuel || fuelRemaining > 0)) {
        accel += Vect(thrustAccel * cos(angle), thrustAccel * sin(angle));
        if (!infiniteFuel) {
            fuelRemaining -= deltaTime;
        }
        
        starTime += deltaTime;
    
        if (starTime > starCreateDelay) {
            starTime -= starCreateDelay;
            parent->entities->add(new Star(parent));
        }
    }
    if (fuelRemaining < 0) {
        fuelRemaining = 0;
    }
    
    // Lift
    float lift = liftAccel(windAngle() + rad(wingAngle));
    accel += Vect(0, 0, 1).crossProduct(vel).unitVector() * lift;
    
    vel += (accel * deltaTime);
    pos += (vel * deltaTime);
    
    if (elevator > 0) {
        angle += rad(turningConst) * deltaTime;
    } else if (elevator < 0) {
        angle -= rad(turningConst) * deltaTime;
    }

    if (!takeoff) {
        if (pos.y > takeoffHeight) {
            takeoff = true;
        }
    }
    
    // Collision with the ground
    if (pos.y <= 0) {
        pos.y = 0;
        vel.y = 0;
        
        if (takeoff && !invincible) {
            running = false;
            parent->hud->add(new Puff(parent, pos, pos));
            record.submit(pos.x);
        }
    }
}

void Penguin::render() {
    Vect relPos = pos - parent->cameraPos;
    
    sprite->render(parent->relToPixel(relPos), util::deg(angle));
}

Penguin::Penguin(World* p) {
    confVar(float, initialFuel);
    confVar(float, penguinSize);
    
    sprite = res.sprite("penguin");
    sprite->setSize(penguinSize, p->metresPerScreen);
    
    pos = Vect();
    vel = Vect();
    angle = 0;
    elevator = 0;
    thrust = 0;
    fuelRemaining = initialFuel;
    running = true;
    takeoff = false;
    parent = p;
}
