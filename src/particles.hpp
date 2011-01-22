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

#ifndef _PARTICLES_HEADER_
#define _PARTICLES_HEADER_

#include <vector>
#include <SFML/Graphics.hpp>

#include "util.hpp"
#include "entities.hpp"
class World;

class Particle: public Entity {
    protected:
        sf::Sprite sprite;
        World *parent;
        Vect pos;
        float angle;
        float size;
        float life;
        
    public:
        virtual void render();
        virtual void doPhysics(float deltaTime);
        virtual bool alive();
        Particle();
};

class Star: public Particle {
    protected:
        Vect vel;
        float angularSpeed;
        
    public:
        void doPhysics(float deltaTime);
        Star(World *p);
};

class Puff: public Particle {
    public:
        void render();
        Puff(World *p, Vect pos1, Vect pos2);
};

#endif // _PARTICLES_HEADER_
