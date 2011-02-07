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

#ifndef _MAIN_HEADER_
#define _MAIN_HEADER_

#include <string>
#include <SFML/Graphics.hpp>
#include "ConfigFile/ConfigFile.h"

#include "apps.hpp"
#include "resources.hpp"
#include "config.h"

extern sf::RenderWindow window;
extern ResourceManager res;
extern AppManager apps;

extern ConfigFile conf;
extern ConfigFile save;
extern std::wstring introText;

void resetWorld();

#ifdef CONFIG_STATIC
    #define confVar(t,x) static t x = conf.read<t>(#x)
#else
    #define confVar(t,x) t x = conf.read<t>(#x)
#endif

// For convenience, use this command to list all configuration keys used
// in this project:
//   grep confVar -h * | grep -v "#define" | cut -f 2 -d , | cut -f 1 -d ')' | sort -u

#endif // _MAIN_HEADER_
