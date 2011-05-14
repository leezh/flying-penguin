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

class FlightInfo: public Entity {
    protected:
        World* parent;
        String* dataText;
        String* labelText;
    public:
        FlightInfo(World *p) {
            parent = p;
            labelText = res.string("regular");
            dataText = res.string("large");
        }
        void render() {
            confVar(int, hudMargin);
            confVar(float, flightInfoCol1);
            confVar(float, flightInfoCol2);
            confVar(float, flightInfoRow1);
            std::string dataStr;
            Vect labelPos(hudMargin, hudMargin);
            Vect dataPos(hudMargin, hudMargin + flightInfoRow1);
            
            labelText->render(labelPos, "Wind Speed", 0.f, 0.f);
            dataStr = util::to_string(parent->penguin->windSpeed(), 1) + "m/s";
            dataText->render(dataPos, dataStr, 0.f, 0.f);
            
            labelPos.x += flightInfoCol1;
            dataPos.x += flightInfoCol1;
            labelText->render(labelPos, "Fuel Remaining", 0.f, 0.f);
            dataStr = util::to_string(parent->penguin->fuelRemaining, 1) + "s";
            dataText->render(dataPos, dataStr, 0.f, 0.f);
            
            labelPos.x += flightInfoCol2;
            dataPos.x += flightInfoCol2;
            labelText->render(labelPos, "Altitude", 0.f, 0.f);
            dataStr = util::to_string(parent->penguin->pos.y, 0) + "m";
            dataText->render(dataPos, dataStr, 0.f, 0.f);
        }
        bool alive() {
            return true;
        }
};

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
    
    background.add(new Background());
    
    for (int i = 0; i < cloudCount; i++) {
        background.add(new Cloud());
    }
    
    penguin = new Penguin();
    entities.add(penguin);
    
    for (int i = 0; i < birdCountStart; i++) {
        entities.add(new Bird(true));
    }
    
    foreground.add(new Fish(0));
    foreground.add(new FlightInfo(this));
    
    
    res.playMusic("mushroom-dance.ogg", 50);
    
    return true;
}

void World::quit() {
    background.clear();
    entities.clear();
    foreground.clear();
}

void World::render() {
    cameraPos = penguin->pos;
    cameraPos.x += metresPerScreen / 4;
    
    background.render();
    entities.render();
    foreground.render();
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
    
    float frameTime = window.GetFrameTime();
    
    background.doPhysics(frameTime);
    entities.doPhysics(frameTime);
    foreground.doPhysics(frameTime);
    
    render();
    window.Display();
    
}

World world;
