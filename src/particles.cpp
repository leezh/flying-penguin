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

Particle::Particle(): sprite() {
    life = 0;
}

void Particle::doPhysics(float deltaTime) {
    life -= deltaTime;
}

void Particle::render() {
    float pixelSize = parent->metresToPixel(size);
    sprite.Resize(pixelSize, pixelSize);
    
    Vect relPos = pos - parent->cameraPos;
    sf::Vector2f pixelPos(parent->metresToPixel(relPos.x), parent->metresToPixel(relPos.y));
    pixelPos.x += window.GetWidth() / 2;
    pixelPos.y = window.GetHeight() / 2 - pixelPos.y;
    sprite.SetPosition(pixelPos);
    
    sprite.SetRotation(util::deg(angle));
    
    window.Draw(sprite);
}

bool Particle::alive() {
    return life > 0;
}

Star::Star(World* p): Particle() {
    confVar(float, starSpeedMax);
    confVar(float, starSpeedMin);
    confVar(float, starAngularSpeedMax);
    confVar(float, starAngularSpeedMin);
    confVar(float, starSizeMax);
    confVar(float, starSizeMin);
    confVar(float, starTTL);
    
    parent = p;
    pos = parent->penguin->pos;
    float speed = (starSpeedMax - starSpeedMin) * util::rnd() + starSpeedMin;
    vel = Vect(-speed * cos(parent->penguin->angle), speed * sin(parent->penguin->angle));
    
    angle = (PI * 2 * util::rnd()) - PI;
    angularSpeed = util::rad((starAngularSpeedMax - starAngularSpeedMin) * util::rnd() + starAngularSpeedMin);
    
    sprite.SetImage(res.img("star"));
    sprite.SetCenter(sprite.GetSubRect().GetWidth() / 2, sprite.GetSubRect().GetHeight() / 2);
    
    size = (starSizeMax - starSizeMin) * util::rnd() + starSizeMin;
    life = starTTL;
}

void Star::doPhysics(float deltaTime) {
    life -= deltaTime;
    
    pos += vel * deltaTime;
    angle += angularSpeed * deltaTime;
}

void Puff::render() {
    confVar(float, puffTTL);
    
    sprite.SetColor(sf::Color(255, 255, 255, 255 * life / puffTTL));
    Particle::render();
}

Puff::Puff(World *p, Vect pos1, Vect pos2): Particle() {
    confVar(float, puffSize);
    confVar(float, puffTTL);
    
    parent = p;
    pos = (pos1 + pos2) / 2;
    
    sprite.SetImage(res.img("puff"));
    sprite.SetCenter(sprite.GetSubRect().GetWidth() / 2, sprite.GetSubRect().GetHeight() / 2);
    
    angle = (PI * 2 * util::rnd()) - PI;
    size = puffSize;
    life = puffTTL;
}
