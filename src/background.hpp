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

#ifndef _BACKGROUND_HEADER_
#define _BACKGROUND_HEADER_

#include <SFML/Graphics.hpp>

#include "util.hpp"
#include "entities.hpp"
class World;

class Background {
    public:
        World *parent;
        sf::Color skyColour;
        sf::Color groundColour;
        
        void render();
        Background(World *p);
};

class Cloud: public Entity {
    protected:
        Sprite* sprite;
        World *parent;
        Vect pos;
        int type;
        
    public:
        void render();
        bool alive() {return true;}
        Cloud(World *p);
};

#endif // _BACKGROUND_HEADER_
