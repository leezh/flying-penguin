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
#include "world.hpp"
#include "background.hpp"
using namespace std;

Background::Background() {
    groundColour = util::to_colour(conf.read<string>("groundColour"));
    skyColour = util::to_colour(conf.read<string>("skyColour"));
}

void Background::render() {
    float w = (float)window.GetWidth();
    float h = (float)window.GetHeight();
    sf::Shape sky = sf::Shape::Rectangle(0, 0, w, h, skyColour);
    
    float groundH = h/2 + world.metresToPixel(world.cameraPos.y);
    sf::Shape ground = sf::Shape::Rectangle(0, groundH, w, h, groundColour);
    
    window.Draw(sky);
    window.Draw(ground);
}

Cloud::Cloud(): sprite() {
    confVar(float, cloudSize);
    
    pos.x = world.cameraPos.x + util::rnd() * world.metresPerScreen * 2 - world.metresPerScreen;
    pos.y = world.cameraPos.y + util::rnd() * world.metresPerScreen * 2 - world.metresPerScreen;
    
    sprite = res.sprite("cloud");
    sprite->setSize(cloudSize, world.metresPerScreen);
    type = floor(util::rnd() * 4);
}

void Cloud::render() {
    Vect relPos = pos - world.cameraPos;
    
    if (relPos.x > world.metresPerScreen) {
        pos.x -= world.metresPerScreen * 2;
        pos.y = world.cameraPos.y + util::rnd() * world.metresPerScreen * 2 - world.metresPerScreen;
        type = floor(util::rnd() * 4);
    }
    if (relPos.x < -world.metresPerScreen) {
        pos.x += world.metresPerScreen * 2;
        pos.y = world.cameraPos.y + util::rnd() * world.metresPerScreen * 2 - world.metresPerScreen;
        type = floor(util::rnd() * 4);
    }
    
    if (relPos.y > world.metresPerScreen) {
        pos.y -= world.metresPerScreen * 2;
        pos.x = world.cameraPos.x + util::rnd() * world.metresPerScreen * 2 - world.metresPerScreen;
        type = floor(util::rnd() * 4);
    }
    if (relPos.y < -world.metresPerScreen) {
        pos.y += world.metresPerScreen * 2;
        pos.x = world.cameraPos.x + util::rnd() * world.metresPerScreen * 2 - world.metresPerScreen;
        type = floor(util::rnd() * 4);
    }
    
    sprite->render(world.relToPixel(relPos), 0.f, type);
}
