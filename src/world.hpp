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

#ifndef _WORLD_HEADER_
#define _WORLD_HEADER_

#include <string>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "main.hpp"
#include "util.hpp"
#include "penguin.hpp"
#include "entities.hpp"
#include "background.hpp"

class World {
    public:
        Vect cameraPos;
        float metresPerScreen;
        Background *background;
        EntityManager *entities;
        EntityManager *hud;
        Penguin *penguin;
        
        inline float metresToPixel(float metres) {
            return metres / metresPerScreen * std::max(window.GetWidth(), window.GetHeight());
        }
        
        inline Vect metresToPixel(Vect metres) {
            return metres / metresPerScreen * std::max(window.GetWidth(), window.GetHeight());
        }
        
        inline Vect relToPixel(Vect pos) {
            pos = metresToPixel(pos);
            pos.x += window.GetWidth() / 2;
            pos.y = window.GetHeight() / 2 - pos.y;
            return pos;
        }
        
        World();
        ~World();
        void render();
        void doPhysics(float deltaTime);
};

extern World *world;

#endif // _WORLD_HEADER_
