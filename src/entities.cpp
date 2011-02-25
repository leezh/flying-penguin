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

#include "world.hpp"
#include "entities.hpp"
using namespace std;

void Text::render() {
    text->render(pos, str, cx, cy);
}

Text::Text(std::string style) {
    text = res.string(style);
    cx = 0.f;
    cy = 0.f;
    pos = Vect();
}

TextBox::TextBox(std::string style): Text(style), selColour() {
    selStart = 0;
    selEnd = 0;
}

void TextBox::render() {
    text->render(pos, editText, cx, cy, true);
    
    sf::FloatRect rect = text->getRect();
    int pixelStart = rect.Left + text->getCharPos(min(selStart, selEnd));
    int pixelEnd = rect.Left + text->getCharPos(max(selStart, selEnd)) + 1;
    sf::Shape box = sf::Shape::Rectangle(pixelStart, rect.Top, pixelEnd, rect.Bottom, selColour);
    window.Draw(box);
    
    text->render(pos, editText, cx, cy);
}

void TextBox::handleEvent(sf::Event &Event) {
    if (Event.Type == sf::Event::TextEntered) {
        if (Event.Text.Unicode < 32 || Event.Text.Unicode > 127) return;
        if (selStart != selEnd) {
            editText.erase(min(selStart, selEnd), abs(selStart - selEnd));
            selEnd = min(selStart, selEnd);
            selStart = selEnd;
        }
        editText.insert(selStart, string(1, char(Event.Text.Unicode)));
        selEnd++;
        selStart = selEnd;
    }
    if (Event.Type == sf::Event::KeyPressed) {
        switch (Event.Key.Code) {
            case sf::Key::Left:
                selEnd--;
                break;
            case sf::Key::Right:
                selEnd++;
                break;
            case sf::Key::Home:
                selEnd = 0;
                break;
            case sf::Key::End:
                selEnd = editText.length();
                break;
            case sf::Key::Back:
                if (selStart != selEnd) {
                    editText.erase(min(selStart, selEnd), abs(selStart - selEnd));
                    selEnd = min(selStart, selEnd);
                    selStart = selEnd;
                } else if (selEnd > 0) {
                    selEnd--;
                    editText.erase(selEnd, 1);
                    selStart = selEnd;
                }
                break;
            default:
                return;
        }
        if (!Event.Key.Shift)
            selStart = selEnd;
    }
    if (selEnd > editText.length()) selEnd = editText.length();
    if (selStart > editText.length()) selStart = editText.length();
    if (selEnd < 0) selEnd = 0;
    if (selStart < 0) selStart = 0;
}

void TextBox::selectAll() {
    selStart = 0;
    selEnd = editText.length();
}

void EntityManager::add(Entity* p) {
    entityArray.push_back(p);
}

void EntityManager::doPhysics(float deltaTime) {
    for (unsigned int i = 0; i < entityArray.size(); i++) {
        entityArray[i]->doPhysics(deltaTime);
    }
    
    unsigned int i = 0;
    while (i < entityArray.size()) {
        if (!(entityArray[i]->alive())) {
            delete (entityArray[i]);
            entityArray.erase(entityArray.begin() + i);
        } else {
            i++;
        }
    }
}

void EntityManager::render() {
    vector<Entity*>::iterator it;
    for (it = entityArray.begin(); it != entityArray.end(); it++) {
        (*it)->render();
    }
}

void EntityManager::clear() {
    vector<Entity*>::iterator it;
    while (entityArray.begin() != entityArray.end()) {
        delete (*entityArray.begin());
        entityArray.erase(entityArray.begin());
    }
}

EntityManager::EntityManager() {}
EntityManager::~EntityManager() {clear();}
