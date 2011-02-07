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

#include <fstream>
#include <cstdlib>
#include <ctime>

#include "main.hpp"
#include "entities.hpp"
#include "world.hpp"
#include "record.hpp"

using namespace std;
Record record;

class WindowShade: public Entity {
    private:
        sf::Shape box;
        
    public:
        void render() {
            box = sf::Shape::Rectangle(0, 0, window.GetWidth(), window.GetHeight(), util::to_colour(conf.read<string>("shadeColour")));
            window.Draw(box);
        }
        bool alive() {return true;}
};

class NameEditor: public TextBox {
    public:
        NameEditor(): TextBox() {
            text.SetColor(util::to_colour(conf.read<string>("shadeTextColour")));
            selColour = util::to_colour(conf.read<string>("shadeColour"));
            align = CENTER;
        }
        void render() {
            confVar(float, textSizeLarge);
            pos = Vect(window.GetWidth() / 2, window.GetHeight() / 2 + textSizeLarge);
            TextBox::render();
        }
};

class RecordWinBanner: public LargeText {
    public:
        RecordWinBanner(): LargeText("New Record: " + util::to_string(record.getRecordDist(), 0) + "m!", CENTER, CENTER) {
            text.SetColor(util::to_colour(conf.read<string>("shadeTextColour")));
        }
        void render() {
            pos = Vect(window.GetWidth() / 2, window.GetHeight() / 2);
            Text::render();
        }
};


class ShadeRestartLabel: public Text {
    public:
        ShadeRestartLabel(): Text() {
            confVar(float, hudMargin);
            text.SetText("Press ENTER to restart");
            pos = Vect(hudMargin, hudMargin);
            text.SetColor(util::to_colour(conf.read<string>("shadeTextColour")));
        }
};

class RecordWin: public App {
    public:
        EntityManager *entities;
        NameEditor *textBox;
        bool init() {
            entities = new EntityManager;
            entities->add(new WindowShade());
            entities->add(new RecordWinBanner());
            entities->add(new ShadeRestartLabel());
            textBox = new NameEditor();
            entities->add(textBox);
            
            textBox->editText = record.getRecordName();
            textBox->selectAll();
            return true;
        }
        void loop() {
            sf::Event Event;
            while (window.GetEvent(Event)){
                if (Event.Type == sf::Event::Closed) {
                    apps.deactivateAll();
                }
                if (Event.Type == sf::Event::Resized) {
                    window.GetDefaultView().SetFromRect(sf::FloatRect(0, 0, Event.Size.Width, Event.Size.Height));
                }
                if (Event.Type == sf::Event::KeyPressed) {
                    switch (Event.Key.Code) {
                        case sf::Key::Escape:
                            apps.deactivateAll();
                            return;
                            break;
                        case sf::Key::Return:
                            save.add("name", textBox->editText);
                            record.submit(record.getRecordDist(), true);
                            resetWorld();
                            apps.deactivate();
                            return;
                            break;
                    }
                }
                textBox->handleEvent(Event);
            }
            world->doPhysics(window.GetFrameTime());
            entities->doPhysics(window.GetFrameTime());
            world->render();
            entities->render();
            
            window.Display();
        }
        void quit() {
            delete entities;
        }
} recordWin;

Record::Record() {
    #ifdef HIGHSCORE_PATH
    ifstream file(HIGHSCORE_PATH);
    if (file) {
        getline(file, username);
        file >> distance;
        file >> time;
        file.close();
    } else {
        username = "Nobody";
        distance = 0;
        time = 0;
    }
    #endif
}

bool Record::submit(float dist, bool internal) {
    if (dist >= distance) {
        username = save.read("name", string("Unknown Penguin"));
        distance = dist;
        time = std::time(NULL);
        #ifdef HIGHSCORE_PATH
        ofstream file(HIGHSCORE_PATH, ios_base::trunc);
        if (file) {
            file << username << endl;
            file << distance << endl;
            file << time << endl;
            file.close();
        }
        if (!internal) {
            apps.activate(&recordWin, false);
        }
        #endif
        return true;
    }
    return false;
}

std::string Record::getRecord() {
    return util::to_string(distance, 0) + "m by " + username;
}

std::string Record::getRecordDate(std::string format) {
    char buf[128];
    strftime(buf, 128, format.c_str(), std::localtime(&time));
    return std::string(buf);
}
