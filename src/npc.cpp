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

Bird::Bird (World *p, bool anywhere): sprite() {
    confVar(float, metresPerScreen);
    confVar(float, birdAltitudeMin);
    confVar(float, birdAltitudeMax);
    confVar(float, birdSpeedMin);
    confVar(float, birdSpeedMax);
    confVar(float, birdAnimDuration);
    
    parent = p;
    sprite.SetImage(res.img("bird"));
    speed = util::rnd() * (birdSpeedMax - birdSpeedMin) + birdSpeedMin;
    clipWidth = sprite.GetSubRect().GetWidth() / 4;
    sprite.SetCenter(clipWidth / 2, sprite.GetSubRect().GetHeight() / 2);
    
    setFrame(0);
    hit = false;
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

void Bird::setFrame(int frame) {
    if (frame < 0 || frame > 3) {frame = 0;}
    
    sf::IntRect box = sprite.GetSubRect();
    box.Left = frame * clipWidth;
    box.Right = box.Left + clipWidth;
    sprite.SetSubRect(box);
}

void Bird::render() {
    confVar(float, birdSize);
    
    sprite.Resize(parent->metresToPixel(birdSize), parent->metresToPixel(birdSize));
    
    Vect relPos = pos - parent->cameraPos;
    sf::Vector2f pixelPos(parent->metresToPixel(relPos.x), parent->metresToPixel(relPos.y));
    pixelPos.x += window.GetWidth() / 2;
    pixelPos.y = window.GetHeight() / 2 - pixelPos.y;
    sprite.SetPosition(pixelPos);
    
    window.Draw(sprite);
}

void Bird::doPhysics(float deltaTime) {
    animTime += deltaTime;
    
    if (hit) {
        setFrame(2);
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
            setFrame(2);
            return;
        }
        
        pos.x += speed * deltaTime;
        while (animTime > birdAnimDuration) {animTime -= birdAnimDuration;}
        float flapAnim = animTime - birdAnimDuration + birdFlapTime * birdFlapCount;
        if (flapAnim < 0) {
            setFrame(0);
        } else {
            int flapFrame = floor(flapAnim / birdFlapTime * 2);
            setFrame(flapFrame % 2);
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

Fish::Fish(World *p, int l): sprite(), arrowSprite() {
    confVar(float, fishDistStart);
    confVar(float, fishDistMultiplier);
    confVar(float, fishAltitudeMin);
    confVar(float, fishAltitudeMax);
    
    parent = p;
    level = l;
    pos.x += parent->penguin->pos.x + fishDistStart + fishDistMultiplier * level;
    if (level != 0) {
        pos.y = util::rnd() * (fishAltitudeMax - fishAltitudeMin) + fishAltitudeMin;
    } else {
        // Makes it easier when taking off.
        pos.y = fishAltitudeMin;
    }
    
    sprite.SetImage(res.img("fish"));
    sprite.SetCenter(sprite.GetSubRect().GetWidth() / 2, sprite.GetSubRect().GetHeight() / 2);
    
    arrowSprite.SetImage(res.img("arrow"));
    arrowSprite.SetCenter(arrowSprite.GetSubRect().GetWidth() / 2, arrowSprite.GetSubRect().GetHeight() / 2);
    
    done = false;
    
    confVar(float, textSizeLarge);
    text.SetFont(res.fnt("regular", textSizeLarge));
    text.SetSize(textSizeLarge);
    text.SetColor(util::hexToColour(conf.read<string>("textColour")));
}

void Fish::render() {
    confVar(float, fishSize);
    
    sprite.Resize(parent->metresToPixel(fishSize), parent->metresToPixel(fishSize));
    
    Vect relPos = pos - parent->cameraPos;
    sf::Vector2f pixelPos(parent->metresToPixel(relPos.x), parent->metresToPixel(relPos.y));
    pixelPos.x += window.GetWidth() / 2;
    pixelPos.y = window.GetHeight() / 2 - pixelPos.y;
    sprite.SetPosition(pixelPos);
    
    if (pixelPos.x > window.GetWidth()) {
        confVar(float, arrowMargin);
        confVar(float, arrowSize);
        
        sf::Vector2f arrowPos = sf::Vector2f(window.GetWidth() - arrowMargin, pixelPos.y);
        
        if (arrowPos.y < arrowMargin) {
            arrowPos.y = arrowMargin;
        }
        if (arrowPos.y > window.GetHeight() - arrowMargin) {
            arrowPos.y = window.GetHeight() - arrowMargin;
        }
        
        sf::Vector2f arrowRelation(pixelPos - arrowPos);
        if (arrowRelation.x == 0 && arrowRelation.y == 0) {
            arrowSprite.SetRotation(0);
        } else {
            arrowSprite.SetRotation(util::deg(atan2(-arrowRelation.y, arrowRelation.x)));
        }
        
        float scale = parent->metresToPixel(arrowSize) / arrowSprite.GetImage()->GetWidth() * 2;
        arrowSprite.SetScale(scale, scale);
        
        arrowSprite.SetPosition(arrowPos);
        window.Draw(arrowSprite);
        
        text.SetText(util::to_string(pos.x - parent->penguin->pos.x, 0) + "m");
        sf::FloatRect rect = text.GetRect();
        text.SetCenter(floor(rect.GetWidth()), floor(rect.GetHeight() / 2));
        text.SetPosition(floor(arrowPos.x - parent->metresToPixel(arrowSize)), floor(arrowPos.y));
        window.Draw(text);
    }
    
    window.Draw(sprite);
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
