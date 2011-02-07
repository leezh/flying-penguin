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

Bird::Bird (World *p, bool anywhere) {
    confVar(float, metresPerScreen);
    confVar(float, birdAltitudeMin);
    confVar(float, birdAltitudeMax);
    confVar(float, birdSpeedMin);
    confVar(float, birdSpeedMax);
    confVar(float, birdAnimDuration);
    confVar(float, birdSize);
    
    parent = p;
    speed = util::rnd() * (birdSpeedMax - birdSpeedMin) + birdSpeedMin;
    
    sprite = res.sprite("bird");
    sprite->setSize(birdSize, p->metresPerScreen);
    
    hit = false;
    frame = 0;
    animTime = birdAnimDuration * util::rnd();
    if (anywhere) {
        pos.x = parent->cameraPos.x + util::rnd() * metresPerScreen * 2 - metresPerScreen;
        pos.y = util::rnd() * (birdAltitudeMax - birdAltitudeMin) + birdAltitudeMin;
    } else {
        switch (int(floor(util::rnd() * 2))) {
            case 0:
                pos.x = parent->cameraPos.x + metresPerScreen;
                pos.y = util::rnd() * (birdAltitudeMax - birdAltitudeMin) + birdAltitudeMin;
                break;
            case 1:
                pos.x = parent->cameraPos.x - metresPerScreen;
                pos.y = util::rnd() * (birdAltitudeMax - birdAltitudeMin) + birdAltitudeMin;
                break;
        }
    }
}

void Bird::render() {
    Vect relPos = pos - parent->cameraPos;
    
    sprite->render(parent->relToPixel(relPos), 0.f, frame);
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
        
        if (Vect(pos - parent->penguin->pos).magnitude() <= birdSize / 2 + penguinSize / 2) {
            hit = true;
            parent->penguin->fuelRemaining -= birdPenalty;
            parent->hud->add(new Puff(parent, pos, parent->penguin->pos));
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
    confVar(float, metresPerScreen);
    
    if (abs(pos.x - parent->cameraPos.x) > metresPerScreen) {
        parent->entities->add(new Bird(parent, false));
        return false;
    }
    
    return true;
}

Fish::Fish(World *p, int l) {
    confVar(float, fishDistStart);
    confVar(float, fishDistMultiplier);
    confVar(float, fishAltitudeMin);
    confVar(float, fishAltitudeMax);
    confVar(float, textSizeLarge);
    confVar(float, fishSize);
    confVar(float, arrowSize);
    
    parent = p;
    level = l;
    pos.x += parent->penguin->pos.x + fishDistStart + fishDistMultiplier * level;
    if (level != 0) {
        pos.y = util::rnd() * (fishAltitudeMax - fishAltitudeMin) + fishAltitudeMin;
    } else {
        // Makes it easier when taking off.
        pos.y = fishAltitudeMin;
    }
    
    sprite = res.sprite("fish");
    sprite->setSize(fishSize, p->metresPerScreen);
    arrowSprite = res.sprite("arrow");
    arrowSprite->setSize(arrowSize, p->metresPerScreen);
    
    text.SetFont(res.font("regular", textSizeLarge));
    text.SetSize(textSizeLarge);
    text.SetColor(util::to_colour(conf.read<string>("textColour")));
    
    done = false;
}

void Fish::render() {
    Vect relPos = pos - parent->cameraPos;
    Vect pixelPos = parent->relToPixel(relPos);
    
    if (pixelPos.x > window.GetWidth()) {
        confVar(float, arrowMargin);
        confVar(float, arrowSize);
        
        Vect arrowPos(window.GetWidth() - arrowMargin - parent->metresToPixel(arrowSize) / 2, pixelPos.y);
        
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
        
        text.SetText(util::to_string(pos.x - parent->penguin->pos.x, 0) + "m");
        sf::FloatRect rect = text.GetRect();
        text.SetCenter(floor(rect.GetWidth()), floor(rect.GetHeight() / 2));
        text.SetPosition(floor(arrowPos.x - parent->metresToPixel(arrowSize) / 2), floor(arrowPos.y));
        window.Draw(text);
    }
    
    sprite->render(pixelPos);
}

void Fish::doPhysics(float deltaTime) {
    confVar(float, penguinSize);
    confVar(float, fishReward);
    confVar(float, fishSize);
    confVar(float, fishMissDist);
    confVar(float, birdCountMultiplier);
    
    if (Vect(pos - parent->penguin->pos).magnitude() <= fishSize / 2 + penguinSize / 2) {
        done = true;
        parent->penguin->fuelRemaining += fishReward;
        parent->entities->add(new Fish(parent, level + 1));
        for (int i = 0; i < birdCountMultiplier; i++) {
            parent->entities->add(new Bird(parent, false));
        }
    }
    
    if (parent->penguin->pos.x - pos.x > fishMissDist) {
        done = true;
        parent->entities->add(new Fish(parent, level + 1));
    }
}

bool Fish::alive() {return !done;}
