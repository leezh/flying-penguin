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

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <physfs.h>
#include "main.hpp"

void Sprite::load(std::string buffer) {
    std::vector<string> lines;
    util::tokenise(buffer, lines, "\n");
    std::vector<string>::iterator it;
    for (it = lines.begin(); it != lines.end(); it++) {
        std::vector<string> tokens;
        util::tokenise((*it), tokens);
        if (tokens.size() == 7) {
            sprites.push_back(new sf::Sprite(*res.image(tokens[0])));
            int x1 = util::from_string<int>(tokens[1]);
            int y1 = util::from_string<int>(tokens[2]);
            int x2 = util::from_string<int>(tokens[3]);
            int y2 = util::from_string<int>(tokens[4]);
            float cx = util::from_string<float>(tokens[5]);
            float cy = util::from_string<float>(tokens[6]);
            sprites[sprites.size() - 1]->SetSubRect(sf::IntRect(x1, y1, x2, y2));
            sprites[sprites.size() - 1]->SetCenter((x2 - x1) * cx, (y2 - y1) * cy);
        }
    }
}

void Sprite::unload() {
    sprites.clear();
}

void Sprite::setSize(float s, float metresPerScreen) {
    size = s;
    recalcSize(metresPerScreen);
}

void Sprite::recalcSize(float metresPerScreen) {
    std::vector<sf::Sprite*>::iterator it;
    int screenSize = std::max(window.GetWidth(), window.GetHeight());
    if (size == -1) return;
    for (it = sprites.begin(); it != sprites.end(); it++) {
        float scale = (size * screenSize) / ((*it)->GetSubRect().GetWidth() * metresPerScreen);
        (*it)->SetScale(scale, scale);
    }
}

void Sprite::render(Vect pos, float angle, int frame) {
    if (frame < 0 || frame >= sprites.size()) frame = 0;
    if (sprites.size() == 0) return;
    sprites[frame]->SetPosition(int(floor(pos.x)), int(floor(pos.y)));
    sprites[frame]->SetRotation(angle);
    
    window.Draw(*sprites[frame]);
}

bool operator< (const FontDesc &f1, const FontDesc &f2) {
    if (f1.name < f2.name) return true;
    if (f1.name == f2.name) return (f1.size < f2.size);
    return false;
}

sf::Image* ResourceManager::image(std::string name) {
    name = "images/" + name;
    if (imageMap.find(name) == imageMap.end()) {
        imageMap[name] = new sf::Image();
        
        bool loaded = false;
        PHYSFS_File* file = PHYSFS_openRead(name.c_str());
        if(file != NULL) {
            PHYSFS_sint64 size = PHYSFS_fileLength(file);
            char *buffer = new char [size];
            int count = PHYSFS_read(file, buffer, size, 1);
            if (count == 1) {
                if (imageMap[name]->LoadFromMemory(buffer, size)) {
                    loaded = true;
                }
            }
            delete[] buffer;
            PHYSFS_close(file);
        }
        
        if (!loaded) {
            std::cerr << "Image \"" << name << "\" could not be loaded" << std::endl;
            // Prevents segfaults
            imageMap[name]->Create(8, 8);
        }
    }
    return imageMap[name];
}

sf::Font& ResourceManager::font(std::string name, float s) {
    name = "fonts/" + name + ".ttf";
    std::vector<std::string>::iterator extIt;
    std::vector<std::string>::iterator dirIt;
    FontDesc desc(name, s);
    
    if (fontMap.find(desc) == fontMap.end()) {
        fontMap[desc] = new sf::Font();
        
        bool loaded = false;
        PHYSFS_File* file = PHYSFS_openRead(name.c_str());
        if(file != NULL) {
            PHYSFS_sint64 size = PHYSFS_fileLength(file);
            char *buffer = new char [size];
            int count = PHYSFS_read(file, buffer, size, 1);
            if (count == 1) {
                if (fontMap[desc]->LoadFromMemory(buffer, size, s)) {
                    loaded = true;
                }
            }
            delete[] buffer;
            PHYSFS_close(file);
        }
        
        if (!loaded) {
            std::cerr << "Font \"" << name << "\" could not be loaded" << std::endl;
        }
    }
    return *fontMap[desc];
}

Sprite* ResourceManager::sprite(std::string name) {
    name = "sprites/" + name;
    if (spriteMap.find(name) == spriteMap.end()) {
        spriteMap[name] = new Sprite();
        
        bool loaded = false;
        PHYSFS_File* file = PHYSFS_openRead(name.c_str());
        if(file != NULL) {
            PHYSFS_sint64 size = PHYSFS_fileLength(file);
            char *buffer = new char [size];
            int count = PHYSFS_read(file, buffer, size, 1);
            if (count == 1) {
                spriteMap[name]->load(buffer);
                loaded = true;
            }
            delete[] buffer;
            PHYSFS_close(file);
        }
        
        if (!loaded) {
            std::cerr << "Sprite \"" << name << "\" could not be loaded" << std::endl;
        }
    }
    return spriteMap[name];
}

void ResourceManager::clear() {
    while (imageMap.begin() != imageMap.end()) {
        delete imageMap.begin()->second;
        imageMap.erase(imageMap.begin());
    }
    
    while (fontMap.begin() != fontMap.end()) {
        delete fontMap.begin()->second;
        fontMap.erase(fontMap.begin());
    }
    
    while (spriteMap.begin() != spriteMap.end()) {
        delete spriteMap.begin()->second;
        spriteMap.erase(spriteMap.begin());
    }
}

void ResourceManager::recalcSizes(float metresPerScreen) {
    std::map<std::string, Sprite*>::iterator it;
    for (it = spriteMap.begin(); it != spriteMap.end(); it++) {
        it->second->recalcSize(metresPerScreen);
    }
}
