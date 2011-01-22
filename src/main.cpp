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

#include "main.hpp"
#include "world.hpp"

using namespace std;

sf::RenderWindow window;
ConfigFile conf;
ConfigFile save;
ResourceManager res;
AppManager apps;
std::wstring introText;

void loadConfig() {
    vector<string> confDirs;
    #ifdef RESOURCE_DIR
    confDirs.push_back(RESOURCE_DIR);
    #endif
    #ifdef RESOURCE_DIR_REL
    confDirs.push_back(RESOURCE_DIR_REL);
    #endif
    #ifdef CONFIG_DIR_ENV
    string homeDir(getenv(CONFIG_DIR_ENV));
    homeDir = homeDir.substr(0, homeDir.find(';'));
    confDirs.push_back(homeDir + "/.flying-penguin/");
    #endif
    
    vector<string>::iterator dirIt;
    for (dirIt = confDirs.begin(); dirIt != confDirs.end(); dirIt++) {
        ifstream file(string(*dirIt + string("config")).c_str());
        if (!file) continue;
        file >> conf;
        file.close();
    }
    
    for (dirIt = confDirs.begin(); dirIt != confDirs.end(); dirIt++) {
        wifstream file(string(*dirIt + string("intro")).c_str());
        if (!file) continue;
        while (!file.eof()) {
            wstring line;
            getline(file, line);
            introText += line + L"\n";
        }
        file.close();
    }
    
    #ifdef CONFIG_DIR_ENV
    ifstream saveFile(string(homeDir + "/.flying-penguin.save").c_str());
    if (saveFile) {saveFile >> save;}
    if (save.read("videoWidth", 0) <= 0) save.remove("videoWidth");     
    if (save.read("videoHeight", 0) <= 0) save.remove("videoHeight");
    #endif
}

void saveConfig() {
    #ifdef CONFIG_DIR_ENV
    string homeDir(getenv(CONFIG_DIR_ENV));
    homeDir = homeDir.substr(0, homeDir.find(';'));
    
    save.add("videoWidth", window.GetWidth());
    save.add("videoHeight", window.GetHeight());
    
    ofstream saveFile(string(homeDir + "/.flying-penguin.save").c_str(), ios_base::trunc);
    if (saveFile) {saveFile << save;}
    #endif
}

void cacheData() {    
    res.img("arrow");
    res.img("bird");
    res.img("cloud");
    res.img("fish");
    res.img("icon");
    res.img("penguin");
    res.img("puff");
    res.img("star");
    res.img("title");
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

int main () {
    srand(time(NULL));
    loadConfig();
    cacheData();
    
    sf::Image &icon = res.img("icon");
    window.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());
    window.Create(sf::VideoMode(save.read("videoWidth", 1024), save.read("videoHeight", 768), 24), "The Flying Penguin");
    window.UseVerticalSync(true);
    
    world = new World();
    apps.activate(&simulation);
    apps.run();
    
	delete world;
	res.clear();
	
	saveConfig();
    window.Close();
	return EXIT_SUCCESS;
}
