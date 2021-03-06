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
#include "record.hpp"
using namespace std;

class FlightInfo: public Entity {
    protected:
        String* dataText;
        String* labelText;
    public:
        FlightInfo() {
            labelText = res.string("regular");
            dataText = res.string("large");
        }
        void render() {
            confVar(int, hudMargin);
            confVar(float, hudFlightCol1);
            confVar(float, hudFlightCol2);
            confVar(float, hudFlightRow1);
            std::string dataStr;
            Vect labelPos(hudMargin, hudMargin);
            Vect dataPos(hudMargin, hudMargin + hudFlightRow1);
            
            labelText->render(labelPos, "Wind Speed", 0.f, 0.f);
            dataStr = util::to_string(world.penguin->windSpeed(), 1) + "m/s";
            dataText->render(dataPos, dataStr, 0.f, 0.f);
            
            labelPos.x += hudFlightCol1;
            dataPos.x += hudFlightCol1;
            labelText->render(labelPos, "Fuel Remaining", 0.f, 0.f);
            dataStr = util::to_string(world.penguin->fuelRemaining, 1) + "s";
            dataText->render(dataPos, dataStr, 0.f, 0.f);
            
            labelPos.x += hudFlightCol2;
            dataPos.x += hudFlightCol2;
            labelText->render(labelPos, "Altitude", 0.f, 0.f);
            dataStr = util::to_string(world.penguin->pos.y, 0) + "m";
            dataText->render(dataPos, dataStr, 0.f, 0.f);
        }
        bool alive() {
            return true;
        }
};

class DistanceInfo: public Entity {
    protected:
        String* dataText;
        String* labelText;
    public:
        DistanceInfo() {
            labelText = res.string("regular");
            dataText = res.string("large");
        }
        void render() {
            confVar(int, hudMargin);
            confVar(float, hudDistanceRow1);
            confVar(float, hudDistanceCol1);
            std::string dataStr;
            Vect dataPos(hudMargin, window.GetHeight() - hudMargin);
            Vect labelPos(hudMargin, window.GetHeight() - hudMargin - hudDistanceRow1);
            
            labelText->render(labelPos, "Record", 0.f, 1.f);
            dataStr = util::to_string(floor(record.getDist()), 0) + "m";
            dataText->render(dataPos, dataStr, 0.f, 1.f);
            
            labelPos.x += hudDistanceCol1;
            dataPos.x += hudDistanceCol1;
            labelText->render(labelPos, "Distance", 0.f, 1.f);
            dataStr = util::to_string(floor(world.penguin->pos.x), 0) + "m";
            dataText->render(dataPos, dataStr, 0.f, 1.f);
        }
        bool alive() {
            return true;
        }
};

class Intro: public Entity {
    protected:
        String* text;
        Sprite* title;
    public:
        Intro() {
            confVar(float, titleSize);
            text = res.string("regular");
            title = res.sprite("title");
            title->setSize(titleSize, world.metresPerScreen);
        }
        void render() {
            Vect pos = world.relToPixel(-world.cameraPos);
            pos.x += window.GetWidth() / 8;
            title->render(pos);
            text->render(pos, introText);
        }
        bool alive() {
            return !world.penguin->isFlying();
        }
};

class RestartInfo: public Entity {
    protected:
        String* text;
    public:
        RestartInfo() {
            text = res.string("regular");
        }
        void render() {
            if (world.penguin->isAlive()) return;
            Vect pos(window.GetWidth() / 2, window.GetHeight() / 3);
            text->render(pos, "Press ENTER to restart", 0.5f, 0.f);
        }
        bool alive() {
            return true;
        }
};

class StallIndicator: public Entity {
    protected:
        String* text;
    public:
        StallIndicator() {
            text = res.string("alert");
        }
        void render() {
            if (!world.penguin->isFlying() || !world.penguin->isStalling()) return;
            Vect pos(window.GetWidth() / 2, window.GetHeight() / 3);
            text->render(pos, "STALL", 0.5f, 0.f);
        }
        bool alive() {
            return world.penguin->isAlive();
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
    foreground.add(new FlightInfo());
    foreground.add(new DistanceInfo());
    foreground.add(new Intro());
    foreground.add(new RestartInfo());
    foreground.add(new StallIndicator());
    
    
    res.playMusic("mushroom-dance.ogg");
    
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
    while (window.PollEvent(Event)){
        if (Event.Type == sf::Event::Closed)
            apps.deactivateAll();
        if (Event.Type == sf::Event::Resized) {
            window.SetView(sf::View(sf::FloatRect(0, 0, Event.Size.Width, Event.Size.Height)));
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
                case sf::Key::M:
                    save.add("mute", !save.read("mute", false));
                    res.updateVolume();
                    break;
            }
        }
    }
    
    const sf::Input& input = window.GetInput();
    penguin->elevator = 0;
    if (input.IsKeyDown(sf::Key::Left)) penguin->elevator += 1;
    if (input.IsKeyDown(sf::Key::Right)) penguin->elevator -= 1;
    penguin->thrust = input.IsKeyDown(sf::Key::Space);
    
    float frameTime = window.GetFrameTime() / 1000.f;
    
    background.doPhysics(frameTime);
    entities.doPhysics(frameTime);
    foreground.doPhysics(frameTime);
    
    render();
    window.Display();
    
}

World world;
