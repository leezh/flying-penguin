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

Background::Background(World *p) {
    parent = p;
    groundColour = util::hexToColour(conf.read<string>("groundColour"));
    skyColour = util::hexToColour(conf.read<string>("skyColour"));
}

void Background::render() {
    float w = (float)window.GetWidth();
    float h = (float)window.GetHeight();
    sf::Shape sky = sf::Shape::Rectangle(0, 0, w, h, skyColour);
    
    float groundH = h/2 + parent->metresToPixel(parent->cameraPos.y);
    sf::Shape ground = sf::Shape::Rectangle(0, groundH, w, h, groundColour);
    
    window.Draw(sky);
    window.Draw(ground);
}

Cloud::Cloud(World *p): sprite() {
    confVar(float, metresPerScreen);
    
    parent = p;
    pos.x = parent->cameraPos.x + util::rnd() * metresPerScreen * 2 - metresPerScreen;
    pos.y = parent->cameraPos.y + util::rnd() * metresPerScreen * 2 - metresPerScreen;
    sprite.SetImage(res.img("cloud"));
    
    resetClip();
}

void Cloud::resetClip() {
    int clipWidth = sprite.GetImage()->GetWidth() / 2;
    int clipHeight = sprite.GetImage()->GetHeight() / 2;
    int type = floor(util::rnd() * 4);
    
    sf::IntRect box;
    box.Left = clipWidth * (type % 2);
    box.Right = box.Left + clipWidth;
    box.Top = clipHeight * floor(type / 2);
    box.Bottom = box.Top + clipHeight;
    sprite.SetSubRect(box);
}

void Cloud::render() {
    confVar(float, metresPerScreen);
    confVar(float, cloudSize);
    Vect relPos = pos - parent->cameraPos;
    
    if (relPos.x > metresPerScreen) {
        pos.x -= metresPerScreen * 2;
        pos.y = parent->cameraPos.y + util::rnd() * metresPerScreen * 2 - metresPerScreen;
        resetClip();
    }
    if (relPos.x < -metresPerScreen) {
        pos.x += metresPerScreen * 2;
        pos.y = parent->cameraPos.y + util::rnd() * metresPerScreen * 2 - metresPerScreen;
        resetClip();
    }
    
    if (relPos.y > metresPerScreen) {
        pos.y -= metresPerScreen * 2;
        pos.x = parent->cameraPos.x + util::rnd() * metresPerScreen * 2 - metresPerScreen;
        resetClip();
    }
    if (relPos.y < -metresPerScreen) {
        pos.y += metresPerScreen * 2;
        pos.x = parent->cameraPos.x + util::rnd() * metresPerScreen * 2 - metresPerScreen;
        resetClip();
    }
    
    sf::Vector2f pixelPos(parent->metresToPixel(relPos.x), parent->metresToPixel(relPos.y));
    pixelPos.x += window.GetWidth() / 2;
    pixelPos.y = window.GetHeight() / 2 - pixelPos.y;
    sprite.SetPosition(pixelPos);
    
    float scale = parent->metresToPixel(cloudSize) / sprite.GetImage()->GetWidth() * 2;
    sprite.SetScale(scale, scale);
    
    window.Draw(sprite);
}
