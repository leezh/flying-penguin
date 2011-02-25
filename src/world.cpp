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
#include "record.hpp"
using namespace std;

World *world;

class SpeedLabel: public Text {
    private:
        World *parent;
    public:
        SpeedLabel(World *p): Text("regular") {
            confVar(float, hudMargin);
            pos = Vect(hudMargin, hudMargin);
            parent = p;
            str = "WindSpeed";
        }
        bool alive() {
            return parent->penguin->isAlive();
        }
};

class SpeedIndicator: public Text {
    private:
        World *parent;
    public:
        SpeedIndicator(World *p): Text("large") {
            confVar(float, hudMargin);
            confVar(float, textSize);
            pos = Vect(hudMargin, hudMargin + textSize);
            parent = p;
        }
        void doPhysics(float deltaTime) {
            str = util::to_string(parent->penguin->windSpeed(), 0) + "m/s";
        }
        bool alive() {
            return parent->penguin->isAlive();
        }
};

class FuelLabel: public Text {
    private:
        World *parent;
    public:
        FuelLabel(World *p): Text("regular") {
            confVar(float, hudMargin);
            confVar(float, hudCol1);
            pos = Vect(hudMargin + hudCol1, hudMargin);
            parent = p;
            str = "Fuel Remaining";
        }
        bool alive() {
            return parent->penguin->isAlive();
        }
};

class FuelIndicator: public Text {
    private:
        World *parent;
    public:
        FuelIndicator(World *p): Text("large") {
            confVar(float, hudMargin);
            confVar(float, hudCol1);
            confVar(float, textSize);
            pos = Vect(hudMargin + hudCol1, hudMargin + textSize);
            parent = p;
        }
        void doPhysics(float deltaTime) {
            str = util::to_string(parent->penguin->fuelRemaining, 1) + "s";
        }
        bool alive() {
            return parent->penguin->isAlive();
        }
};

class AltitudeLabel: public Text {
    private:
        World *parent;
    public:
        AltitudeLabel(World *p): Text("regular") {
            confVar(float, hudMargin);
            confVar(float, hudCol2);
            pos = Vect(hudMargin + hudCol2, hudMargin);
            parent = p;
            str = "Altitude";
        }
        bool alive() {
            return parent->penguin->isAlive();
        }
};

class AltitudeIndicator: public Text {
    private:
        World *parent;
    public:
        AltitudeIndicator(World *p): Text("large") {
            confVar(float, hudMargin);
            confVar(float, hudCol2);
            confVar(float, textSize);
            pos = Vect(hudMargin + hudCol2, hudMargin + textSize);
            parent = p;
        }
        void doPhysics(float deltaTime) {
            str = util::to_string(parent->penguin->pos.y, 0) + "m";
        }
        bool alive() {
            return parent->penguin->isAlive();
        }
};

class DistanceIndicator: public Text {
    private:
        World *parent;
    public:
        DistanceIndicator(World *p): Text("regular") {
            cy = 1.f;
            parent = p;
        }
        void doPhysics(float deltaTime) {
            str = "Distance: " + util::to_string(parent->penguin->pos.x, 0) + "m";
        }
        void render() {
            confVar(float, hudMargin);
            pos = Vect(hudMargin, window.GetHeight() - hudMargin);
            Text::render();
        }
};

class RecordIndicator: public Text {
    private:
        World *parent;
    public:
        RecordIndicator(World *p): Text("regular") {
            cy = 1.f;
            parent = p;
            str = "Record: " + record.getRecord();
        }
        void render() {
            confVar(float, hudMargin);
            confVar(float, textSize);
            
            float posX = hudMargin - parent->metresToPixel(parent->penguin->pos.x);
            float posY = window.GetHeight() - hudMargin - textSize + parent->metresToPixel(parent->penguin->pos.y);
            pos = Vect(posX, posY);
            Text::render();
        }
        bool alive() {
            return parent->penguin->pos.x < parent->metresPerScreen * 2;
        }
};

class Instructions: public Text {
    private:
        World *parent;
    public:
        Instructions(World *p): Text("regular") {
            parent = p;
            str = introText;
        }
        void render() {
            confVar(float, hudMargin);
            confVar(float, titleOffset);
            float posX = window.GetWidth() / 2 - parent->metresToPixel(parent->penguin->pos.x - titleOffset);
            float posY = window.GetHeight() / 2 + parent->metresToPixel(parent->penguin->pos.y);
            pos = Vect(posX, posY);
            Text::render();
        }
        bool alive() {
            return parent->penguin->pos.x < parent->metresPerScreen * 2;
        }
};

class Title: public Entity {
    private:
        World *parent;
        Sprite* sprite;
        
    public:
        Title(World *p) {
            confVar(float, titleSize);
            parent = p;
            sprite = res.sprite("title");
            sprite->setSize(titleSize, p->metresPerScreen);
        }
        void render() {
            confVar(float, titleOffset);
            
            float posX = window.GetWidth() / 2 - parent->metresToPixel(parent->penguin->pos.x - titleOffset);
            float posY = window.GetHeight() / 2 + parent->metresToPixel(parent->penguin->pos.y);
            
            sprite->render(Vect(posX, posY));
        }
        bool alive() {
            return parent->penguin->pos.x < parent->metresPerScreen * 2;
        }
};

class RestartLabel: public Text {
    private:
        World *parent;
    public:
        RestartLabel(World *p): Text("regular") {
            confVar(float, hudMargin);
            pos = Vect(hudMargin, hudMargin);
            parent = p;
            str = "Press ENTER to restart";
        }
        void render() {
            if (!parent->penguin->isAlive()) Text::render();
        }
};

World::World() {
    confVar(int, cloudCount);
    confVar(int, birdCountStart);
    
    cameraPos = Vect(0.f, 0.f);
    metresPerScreen = conf.read<float>("metresPerScreen");
    hud = new EntityManager();
    entities = new EntityManager();
    penguin = new Penguin(this);
    background = new Background(this);
    
    for (int i = 0; i < cloudCount; i++) {
        entities->add(new Cloud(this));
    }
    
    hud->add(new SpeedLabel(this));
    hud->add(new SpeedIndicator(this));
    hud->add(new FuelLabel(this));
    hud->add(new FuelIndicator(this));
    hud->add(new AltitudeLabel(this));
    hud->add(new AltitudeIndicator(this));
    hud->add(new DistanceIndicator(this));
    hud->add(new RecordIndicator(this));
    hud->add(new Instructions(this));
    hud->add(new Title(this));
    hud->add(new RestartLabel(this));
    
    entities->add(new Fish(this, 0));
    
    for (int i = 0; i < birdCountStart; i++) {
        entities->add(new Bird(this, true));
    }
}

World::~World() {
    delete penguin;
    delete background;
    delete hud;
    delete entities;
}

void World::render() {
    // Use this section for camera shakes and whatnot
    cameraPos = penguin->pos;
    cameraPos.x += metresPerScreen / 4;
    
    background->render();
    entities->render();
    penguin->render();
    hud->render();
}

void World::doPhysics(float deltaTime) {
    entities->doPhysics(deltaTime);
    penguin->doPhysics(deltaTime);
    hud->doPhysics(deltaTime);
}
