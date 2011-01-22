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

#ifndef _NPC_HEADER_
#define _NPC_HEADER_

#include <vector>
#include <SFML/Graphics.hpp>

#include "util.hpp"
#include "entities.hpp"
class World;

class Bird: public Entity {
    private:
        sf::Sprite sprite;
        World *parent;
        Vect pos;
        float speed;
        int clipWidth;
        float animTime;
        bool hit;
        void setFrame(int frame);
        
    public:
        void render();
        void respawn();
        void doPhysics(float deltaTime);
        bool alive();
        Bird(World *p, bool anywhere);
};

class Fish: public Entity {
    private:
        sf::Sprite sprite;
        sf::Sprite arrowSprite;
        sf::String text;
        World *parent;
        Vect pos;
        bool done;
        int level;
        
    public:
        Fish(World *p, int level);
        void render();
        void doPhysics(float deltaTime);
        bool alive();
};

#endif // _NPC_HEADER_
