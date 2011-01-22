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

#ifndef _PENGUIN_HEADER_
#define _PENGUIN_HEADER_

#include <SFML/Graphics.hpp>
#include "util.hpp"

class World;

class Penguin {
    private:
        bool running;
        bool takeoff;
        float starTime;
        sf::Sprite sprite;
        
        bool isUnderspeed();
        bool isStalling(float angle);
        float windAngle();
        float liftAccel(float angle);
        World *parent;
        
    public:
        Vect pos;
        Vect vel;
        float angle;
        int elevator;
        bool thrust;
        float fuelRemaining;
        
        bool isAlive();
        bool isFlying();
        float windSpeed();
        
        void doPhysics(float deltaTime);
        void render();
        Penguin(World *p);
};

#endif // _PENGUIN_HEADER_
