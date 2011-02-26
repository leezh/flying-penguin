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

#include <exception>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <physfs.h>
#include "main.hpp"
#include "world.hpp"

using namespace std;

sf::RenderWindow window;
ConfigFile conf;
ConfigFile save;
ResourceManager res;
AppManager apps;
std::string introText;

void loadConfig() {
    PHYSFS_File* file = PHYSFS_openRead("config");
    if(file != NULL) {
        PHYSFS_sint64 size = PHYSFS_fileLength(file);
        char* buffer = new char [size];
        int count = PHYSFS_read(file, buffer, size, 1);
        if (count == 1) {
            stringstream ss;
            ss.str(string(buffer, size));
            ss >> conf;
        }
        delete[] buffer;
        PHYSFS_close(file);
    }
    
    file = PHYSFS_openRead("intro");
    if(file != NULL) {
        PHYSFS_sint64 size = PHYSFS_fileLength(file);
        char* buffer = new char [size];
        int count = PHYSFS_read(file, buffer, size, 1);
        if (count == 1) {
            introText = string(buffer, size);
        }
        delete[] buffer;
        PHYSFS_close(file);
    }
    
    file = PHYSFS_openRead("save");
    if(file != NULL) {
        PHYSFS_sint64 size = PHYSFS_fileLength(file);
        char* buffer = new char [size];
        int count = PHYSFS_read(file, buffer, size, 1);
        if (count == 1) {
            stringstream ss;
            ss.str(string(buffer, size));
            ss >> save;
        }
        delete[] buffer;
        PHYSFS_close(file);
    }
    
    if (save.read("videoWidth", 0) <= 0) save.add("videoWidth", 1024);     
    if (save.read("videoHeight", 0) <= 0) save.add("videoHeight", 768);
}

void saveConfig() {
    PHYSFS_File* file = PHYSFS_openWrite("save");
    if(file != NULL) {
        stringstream ss;
        ss << save;
        string buffer = ss.str();
        
        PHYSFS_sint64 size = buffer.length();
        int count = PHYSFS_write(file, buffer.c_str(), size, 1);
        PHYSFS_close(file);
    }
}

void resetWorld() {
    delete world;
    world = new World();
}

class Simulation: public App {
    public:
        bool init() {
            return true;
        }
        void loop() {
            sf::Event Event;
            const sf::Input& input = window.GetInput();
            while (window.GetEvent(Event)){
                if (Event.Type == sf::Event::Closed)
                    apps.deactivateAll();
                if (Event.Type == sf::Event::Resized) {
                    window.GetDefaultView().SetFromRect(sf::FloatRect(0, 0, Event.Size.Width, Event.Size.Height));
                    res.recalcSizes(world->metresPerScreen);
                }
                if (Event.Type == sf::Event::KeyPressed) {
                    switch (Event.Key.Code) {
                        case sf::Key::Escape:
                        case sf::Key::Q:
                            apps.deactivate();
                            return;
                            break;
                        case sf::Key::Return:
                            resetWorld();
                            break;
                    }
                }
            }
            world->penguin->elevator = 0;
            if (input.IsKeyDown(sf::Key::Left)) world->penguin->elevator += 1;
            if (input.IsKeyDown(sf::Key::Right)) world->penguin->elevator -= 1;
            world->penguin->thrust = input.IsKeyDown(sf::Key::Space);
            world->doPhysics(window.GetFrameTime());
            world->render();
            window.Display();
        }
} simulation;

int main (int argc, char** argv) {
    PHYSFS_init(NULL);
    PHYSFS_setSaneConfig("config", "flying-penguin", NULL, 0, 0);
    #ifdef RESOURCE_DIR
    PHYSFS_addToSearchPath(RESOURCE_DIR, 1);
    #endif
    #ifdef RESOURCE_DIR_REL
    PHYSFS_addToSearchPath(RESOURCE_DIR_REL, 1);
    #endif
    
    srand(time(NULL));
    loadConfig();
    res.cacheData();
    
    sf::Image &icon = *res.image("icon.png");
    window.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());
    window.Create(sf::VideoMode(save.read<int>("videoWidth"), save.read<int>("videoHeight"), 24), "The Flying Penguin");
    window.UseVerticalSync(true);
    
    world = new World();
    apps.activate(&simulation);
    apps.run();
    
	delete world;
	res.clear();
	
	saveConfig();
    window.Close();
    PHYSFS_deinit();
	return EXIT_SUCCESS;
}
