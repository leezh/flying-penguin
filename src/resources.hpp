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

#ifndef _RESOURCES_HEADER_
#define _RESOURCES_HEADER_

#include <string>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

#include "util.hpp"

class ResourceManager;

class Sprite {
    private:
        std::vector<sf::Sprite*> sprites;
        float size;
        
        void load(std::string buffer);
        void unload();
        void recalcSize(float metresPerScreen);
    public:
        // Set to < 0 if you want to manually calculate the pixel size
        void setSize(float s, float metresPerScreen);
        
        void render(Vect pos, float angle = 0.f, int frame = 0);
        friend class ResourceManager;
};

class String {
    private:
        sf::String* str;
        
        void load(std::string buffer);
        void unload();
    public:
        sf::FloatRect getRect();
        int getCharPos(int pos);
        
        void render(Vect pos, std::string text, float cx = 0.f, float cy = 0.f, bool dummy = false);
        friend class ResourceManager;
};

struct FontDesc {
    std::string name;
    float size;
    FontDesc(std::string n, float s): name(n), size(s){}
};
inline bool operator< (const FontDesc &f1, const FontDesc &f2) {
    if (f1.name < f2.name) return true;
    if (f1.name == f2.name) return (f1.size < f2.size);
    return false;
}

class ResourceManager {
    private:
        
        std::map<std::string, sf::Image*> imageMap;
        std::map<FontDesc, sf::Font*> fontMap;
        std::map<std::string, Sprite*> spriteMap;
        std::map<std::string, String*> stringMap;
    public:
        sf::Image* image(std::string name);
        sf::Font* font(std::string name, float size);
        Sprite* sprite(std::string name);
        String* string(std::string name);
        
        void clear();
        void cacheData();
        void recalcSizes(float metresPerScreen);
};

#endif // _RESOURCES_HEADER_
