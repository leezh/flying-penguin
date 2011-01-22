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
#include <SFML/Graphics.hpp>
#include "main.hpp"

bool operator< (const FontDesc &f1, const FontDesc &f2) {
    if (f1.name < f2.name) return true;
    if (f1.name == f2.name) return (f1.size < f2.size);
    return false;
}

sf::Image& ResourceManager::img(std::string name) {
    std::vector<std::string>::iterator extIt;
    std::vector<std::string>::iterator dirIt;
    
    if (imageMap.find(name) == imageMap.end()) {
        imageMap[name] = sf::Image();
        
        bool loaded = false;
        for (dirIt = resourceDirs.begin(); dirIt != resourceDirs.end(); dirIt++) {
            for (extIt = imageExt.begin(); extIt != imageExt.end(); extIt++) {
                if (loaded) break;
                std::string path = (*dirIt) + std::string("images/") + name + (*extIt);
                
                std::ifstream test(path.c_str());
                if (!test) continue;
                
                if (imageMap[name].LoadFromFile(path)) {
                    loaded = true;
                }
            }
        }
        if (!loaded) {
            std::cerr << "Image not found: \"" << name << "\"" << std::endl;
            imageMap[name].Create(1, 1);
        }
    }
    return imageMap[name];
}

sf::Font& ResourceManager::fnt(std::string name, float size) {
    std::vector<std::string>::iterator extIt;
    std::vector<std::string>::iterator dirIt;
    FontDesc desc(name, size);
    
    if (fontMap.find(desc) == fontMap.end()) {
        fontMap[desc] = sf::Font();
        
        bool loaded = false;
        for (dirIt = resourceDirs.begin(); dirIt != resourceDirs.end(); dirIt++) {
            for (extIt = fontExt.begin(); extIt != fontExt.end(); extIt++) {
                if (loaded) break;
                std::string path = (*dirIt) + std::string("fonts/") + name + (*extIt);
                
                std::ifstream test(path.c_str());
                if (!test) continue;
                
                if (fontMap[desc].LoadFromFile(path, size)) {
                    loaded = true;
                }
            }
        }
        if (!loaded) {
            std::cerr << "Font not found: \"" << name << "\"" << std::endl;
        }
    }
    return fontMap[desc];
}

void ResourceManager::clear() {
    while (imageMap.begin() != imageMap.end()) {
        imageMap.erase(imageMap.begin());
    }
    
    while (fontMap.begin() != fontMap.end()) {
        fontMap.erase(fontMap.begin());
    }
}

ResourceManager::ResourceManager() {
    // Load only once
    if (loaded) return;
    
    // Populate the extension list
    imageExt.push_back(".png");
    imageExt.push_back(".tga");
    imageExt.push_back(".jpg");
    imageExt.push_back(".bmp");
    fontExt.push_back(".ttf");
    
    // Populate the resource directories list
    resourceDirs.push_back("");
    #ifdef CONFIG_DIR_ENV
    string homeDir(getenv(CONFIG_DIR_ENV));
    homeDir = homeDir.substr(0, homeDir.find(';'));
    resourceDirs.push_back(homeDir + "/.flying-penguin/");
    #endif
    #ifdef RESOURCE_DIR_REL
    resourceDirs.push_back(RESOURCE_DIR_REL);
    #endif
    #ifdef RESOURCE_DIR
    resourceDirs.push_back(RESOURCE_DIR);
    #endif
    
    loaded = true;
}
