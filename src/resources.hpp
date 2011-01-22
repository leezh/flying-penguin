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


struct FontDesc {
    std::string name;
    float size;
    FontDesc(std::string n, float s): name(n), size(s){}
};
bool operator< (const FontDesc &f1, const FontDesc &f2);

/// The purpose of this class is to load resources in a Quake-ish manner
/// where it looks for the files in different directories, automatically
/// detecting the file extensions so that the main code doesn't need to
/// worry about that.
class ResourceManager {
    private:
        bool loaded;
        std::vector<std::string> resourceDirs;
        std::vector<std::string> imageExt;
        std::map<std::string, sf::Image> imageMap;
        std::vector<std::string> fontExt;
        std::map<FontDesc, sf::Font> fontMap;
    public:
        /// Returns the image of the specified name, loading it to
        /// memory if it hasn't already.
        /// @param name The filename (without file extensions) of the
        ///             image to load
        /// @return The reference to the image loaded
        sf::Image& img(std::string name);
        
        /// Returns the font of the specified name, loading it to
        /// memory if it hasn't already.
        /// @param name The filename (without file extensions) of the
        ///             font to load
        /// @param size The size of the font to load
        /// @return The reference to the image loaded
        sf::Font& fnt(std::string name, float size);
        
        /// Unloads all the loaded files.
        void clear();
        
        /// Generates the list of file extensions to search with as well
        /// as the list of directories to search in.
        ResourceManager();
};

#endif // _RESOURCES_HEADER_
