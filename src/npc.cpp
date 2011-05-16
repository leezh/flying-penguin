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
#include "npc.hpp"
#include "particles.hpp"
#include "world.hpp"
using namespace std;

Bird::Bird (bool anywhere) {
    confVar(float, birdAltitudeMin);
    confVar(float, birdAltitudeMax);
    confVar(float, birdSpeedMin);
    confVar(float, birdSpeedMax);
    confVar(float, birdAnimDuration);
    confVar(float, birdSize);
    
    speed = util::rnd() * (birdSpeedMax - birdSpeedMin) + birdSpeedMin;
    
    sprite = res.sprite("bird");
    sprite->setSize(birdSize, world.metresPerScreen);
    
    hit = false;
    frame = 0;
    animTime = birdAnimDuration * util::rnd();
    if (anywhere) {
        pos.x = world.cameraPos.x + util::rnd() * world.metresPerScreen * 2 - world.metresPerScreen;
        pos.y = util::rnd() * (birdAltitudeMax - birdAltitudeMin) + birdAltitudeMin;
    } else {
        switch (int(floor(util::rnd() * 2))) {
            case 0:
                pos.x = world.cameraPos.x + world.metresPerScreen;
                pos.y = util::rnd() * (birdAltitudeMax - birdAltitudeMin) + birdAltitudeMin;
                break;
            case 1:
                pos.x = world.cameraPos.x - world.metresPerScreen;
                pos.y = util::rnd() * (birdAltitudeMax - birdAltitudeMin) + birdAltitudeMin;
                break;
        }
    }
}

void Bird::render() {
    Vect relPos = pos - world.cameraPos;
    
    sprite->render(world.relToPixel(relPos), 0.f, frame);
}

void Bird::doPhysics(float deltaTime) {
    animTime += deltaTime;
    
    if (hit) {
        frame = 2;
    } else {
        confVar(float, birdAnimDuration);
        confVar(float, birdFlapTime);
        confVar(float, birdFlapCount);
        confVar(float, birdSize);
        confVar(float, birdPenalty);
        confVar(float, penguinSize);
        
        if (Vect(pos - world.penguin->pos).magnitude() <= birdSize / 2 + penguinSize / 2) {
            hit = true;
            world.penguin->fuelRemaining -= birdPenalty;
            world.foreground.add(new Puff(pos, world.penguin->pos));
            res.sound("buzzer.wav")->play();
            frame = 2;
            return;
        }
        
        pos.x += speed * deltaTime;
        while (animTime > birdAnimDuration) {animTime -= birdAnimDuration;}
        float flapAnim = animTime - birdAnimDuration + birdFlapTime * birdFlapCount;
        if (flapAnim < 0) {
            frame = 0;
        } else {
            int flapFrame = floor(flapAnim / birdFlapTime * 2);
            frame = flapFrame % 2;
        }
    }
}

bool Bird::alive() {
    if (abs(pos.x - world.cameraPos.x) > world.metresPerScreen) {
        world.entities.add(new Bird(false));
        return false;
    }
    
    return true;
}

Fish::Fish(int l) {
    confVar(float, fishDistStart);
    confVar(float, fishDistMultiplier);
    confVar(float, fishAltitudeMin);
    confVar(float, fishAltitudeMax);
    confVar(float, fishSize);
    confVar(float, arrowSize);
    
    level = l;
    pos.x += world.penguin->pos.x + fishDistStart + fishDistMultiplier * level;
    if (level != 0) {
        pos.y = util::rnd() * (fishAltitudeMax - fishAltitudeMin) + fishAltitudeMin;
    } else {
        // Makes it easier when taking off.
        pos.y = fishAltitudeMin;
    }
    
    sprite = res.sprite("fish");
    sprite->setSize(fishSize, world.metresPerScreen);
    arrowSprite = res.sprite("arrow");
    arrowSprite->setSize(arrowSize, world.metresPerScreen);
    
    text = res.string("large");
    
    done = false;
}

void Fish::render() {
    Vect relPos = pos - world.cameraPos;
    Vect pixelPos = world.relToPixel(relPos);
    
    if (pixelPos.x > window.GetWidth()) {
        confVar(float, arrowMargin);
        confVar(float, arrowSize);
        
        Vect arrowPos(window.GetWidth() - arrowMargin - world.metresToPixel(arrowSize) / 2, pixelPos.y);
        
        if (arrowPos.y < arrowMargin) {
            arrowPos.y = arrowMargin;
        }
        if (arrowPos.y > window.GetHeight() - arrowMargin) {
            arrowPos.y = window.GetHeight() - arrowMargin;
        }
        
        float angle;
        
        Vect arrowRelation(pixelPos - arrowPos);
        if (arrowRelation.x == 0 && arrowRelation.y == 0) {
            angle = 0.f;
        } else {
            angle = util::deg(atan2(-arrowRelation.y, arrowRelation.x));
        }
        
        arrowSprite->render(arrowPos, angle);
        
        
        std::string str(util::to_string(pos.x - world.penguin->pos.x, 0) + "m");
        Vect pos(arrowPos.x - world.metresToPixel(arrowSize) / 2, arrowPos.y);
        text->render(pos, str, 1.f, 0.5f);
    }
    
    sprite->render(pixelPos);
}

void Fish::doPhysics(float deltaTime) {
    confVar(float, penguinSize);
    confVar(float, fishReward);
    confVar(float, fishSize);
    confVar(float, fishMissDist);
    confVar(float, birdCountMultiplier);
    
    if (Vect(pos - world.penguin->pos).magnitude() <= fishSize / 2 + penguinSize / 2) {
        done = true;
        world.penguin->fuelRemaining += fishReward;
        world.entities.add(new Fish(level + 1));
        for (int i = 0; i < birdCountMultiplier; i++) {
            world.entities.add(new Bird(false));
        }
        res.sound("point-bell.wav")->play();
    }
    
    if (world.penguin->pos.x - pos.x > fishMissDist) {
        done = true;
        world.entities.add(new Fish(level + 1));
    }
}

bool Fish::alive() {
    return !done;
}
