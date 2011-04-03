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

#include <string>

#include "main.hpp"
#include "particles.hpp"
#include "world.hpp"
using namespace std;

Star::Star() {
    confVar(float, starSpeedMax);
    confVar(float, starSpeedMin);
    confVar(float, starAngularSpeedMax);
    confVar(float, starAngularSpeedMin);
    confVar(float, starSize);
    confVar(float, starTTL);
    
    pos = world.penguin->pos;
    float speed = (starSpeedMax - starSpeedMin) * util::rnd() + starSpeedMin;
    vel = Vect(-speed * cos(world.penguin->angle), speed * sin(world.penguin->angle));
    
    angle = (PI * 2 * util::rnd()) - PI;
    angularSpeed = util::rad((starAngularSpeedMax - starAngularSpeedMin) * util::rnd() + starAngularSpeedMin);
    
    sprite = res.sprite("star");
    sprite->setSize(starSize, world.metresPerScreen);
    
    life = starTTL;
}

void Star::doPhysics(float deltaTime) {
    life -= deltaTime;
    
    pos += vel * deltaTime;
    angle += angularSpeed * deltaTime;
}

bool Star::alive() {
    return life > 0;
}

void Star::render() {
    Vect relPos = pos - world.cameraPos;
    sprite->render(world.relToPixel(relPos), util::deg(angle));
}

Puff::Puff(Vect pos1, Vect pos2) {
    confVar(float, puffSize);
    confVar(float, puffTTL);
    
    pos = (pos1 + pos2) / 2;
    
    sprite = res.sprite("puff");
    sprite->setSize(puffSize, world.metresPerScreen);
    
    angle = (PI * 2 * util::rnd()) - PI;
    life = puffTTL;
}

void Puff::doPhysics(float deltaTime) {
    life -= deltaTime;
}

bool Puff::alive() {
    return life > 0;
}

void Puff::render() {
    Vect relPos = pos - world.cameraPos;
    sprite->render(world.relToPixel(relPos), util::deg(angle));
}
