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

#include "resources.hpp"
#include "util.hpp"
#include "apps.hpp"
#include "entities.hpp"
#include "penguin.hpp"

class World: public App {
    public:
        Vect cameraPos;
        float metresPerScreen;
        
        EntityManager background;
        EntityManager entities;
        EntityManager foreground;
        
        Penguin *penguin;
        
        float metresToPixel(float metres);
        Vect metresToPixel(Vect metres);
        Vect relToPixel(Vect pos);
        
        bool init();
        void quit();
        void render();
        void loop();
};

extern World world;

#endif // _WORLD_HEADER_
