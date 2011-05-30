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

bool Penguin::isStalling() {
    confVar(float, stallThreshold);
    return (stallTime >= stallThreshold);
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
    confVar(float, liftConst);
    
    float speed = windSpeed();
    return speed * speed * liftConst * angle;
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
    confVar(float, stallAngle);
    
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
            world.background.add(new Star());
        }
    }
    if (fuelRemaining < 0) {
        fuelRemaining = 0;
    }
    
    // Lift
    float wAngle = windAngle();
    if (wAngle < rad(stallAngle) && wAngle > -rad(stallAngle)) {
        float lift = liftAccel(wAngle + rad(wingAngle));
        accel += Vect(0, 0, 1).crossProduct(vel).unitVector() * lift;
        stallTime = 0;
    } else {
        stallTime += deltaTime;
    }
    
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
            res.sound("buzzer.wav")->play();
            res.stopMusic();
            world.foreground.add(new Puff(pos, pos));
            if (record.submit(pos.x)) {
                world.foreground.add(new RecordBanner());
                res.sound("applause.wav")->play();
            }
        }
    }
}

void Penguin::render() {
    Vect relPos = pos - world.cameraPos;
    sprite->render(world.relToPixel(relPos), util::deg(angle));
}

Penguin::Penguin() {
    confVar(float, initialFuel);
    confVar(float, penguinSize);
    
    sprite = res.sprite("penguin");
    sprite->setSize(penguinSize, world.metresPerScreen);
    
    pos = Vect();
    vel = Vect();
    angle = 0;
    elevator = 0;
    thrust = 0;
    fuelRemaining = initialFuel;
    starTime = 0;
    running = true;
    takeoff = false;
}
