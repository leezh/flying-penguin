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

#include "world.hpp"
#include "npc.hpp"
#include "background.hpp"
#include "main.hpp"
using namespace std;

float World::metresToPixel(float metres) {
    return metres / metresPerScreen * max(window.GetWidth(), window.GetHeight());
}

Vect World::metresToPixel(Vect metres) {
    return metres / metresPerScreen * max(window.GetWidth(), window.GetHeight());
}

Vect World::relToPixel(Vect pos) {
    pos = metresToPixel(pos);
    pos.x += window.GetWidth() / 2;
    pos.y = window.GetHeight() / 2 - pos.y;
    return pos;
}

bool World::init() {
    confVar(int, cloudCount);
    confVar(int, birdCountStart);
    
    cameraPos = Vect(0.f, 0.f);
    metresPerScreen = conf.read<float>("metresPerScreen");
    
    entities.add(new Background());
    
    for (int i = 0; i < cloudCount; i++) {
        entities.add(new Cloud());
    }
    
    penguin = new Penguin();
    entities.add(penguin);
    
    entities.add(new Fish(0));
    
    for (int i = 0; i < birdCountStart; i++) {
        entities.add(new Bird(true));
    }
    
    
    res.playMusic("mushroom-dance.ogg", 50);
    
    return true;
}

void World::quit() {
    entities.clear();
}

void World::render() {
    cameraPos = penguin->pos;
    cameraPos.x += metresPerScreen / 4;
    
    entities.render();
}

void World::loop() {
    sf::Event Event;
    while (window.GetEvent(Event)){
        if (Event.Type == sf::Event::Closed)
            apps.deactivateAll();
        if (Event.Type == sf::Event::Resized) {
            window.GetDefaultView().SetFromRect(sf::FloatRect(0, 0, Event.Size.Width, Event.Size.Height));
            res.recalcSizes(world.metresPerScreen);
        }
        if (Event.Type == sf::Event::KeyPressed) {
            switch (Event.Key.Code) {
                case sf::Key::Escape:
                    apps.deactivate();
                    return;
                    break;
                case sf::Key::Return:
                    apps.activate(&world);
                    return;
                    break;
            }
        }
    }
    
    const sf::Input& input = window.GetInput();
    penguin->elevator = 0;
    if (input.IsKeyDown(sf::Key::Left)) penguin->elevator += 1;
    if (input.IsKeyDown(sf::Key::Right)) penguin->elevator -= 1;
    penguin->thrust = input.IsKeyDown(sf::Key::Space);
    
    entities.doPhysics(window.GetFrameTime());
    render();
    window.Display();
    
}

World world;
