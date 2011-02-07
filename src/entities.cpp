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

sf::Vector2f Text::doAlignment() {
    sf::FloatRect rect = text.GetRect();
    float centerX, centerY;
    switch (align) {
        default:
        case LEFT:
            centerX = 0;
            break;
        case CENTER:
            centerX = floor(rect.GetWidth() / 2);
            break;
        case RIGHT:
            centerX = floor(rect.GetWidth());
            break;
    }
    switch (valign) {
        default:
        case TOP:
            centerY = 0;
            break;
        case CENTER:
            centerY = floor(rect.GetHeight() / 2);
            break;
        case BOTTOM:
            centerY = floor(rect.GetHeight());
            break;
    }
    text.SetCenter(centerX, centerY);
    text.SetPosition(floor(pos.x), floor(pos.y));
    
    return text.TransformToGlobal(sf::Vector2f(0, 0));
}

void Text::render() {
    doAlignment();
    window.Draw(text);
}

Text::Text(string str, int a, int v, Vect p): text() {
    confVar(float, textSize);
    text.SetFont(res.font("regular", textSize));
    text.SetSize(textSize);
    text.SetColor(util::to_colour(conf.read<string>("textColour")));
    text.SetText(str);
    align = a;
    valign = v;
    pos = p;
}

LargeText::LargeText(string str, int a, int v, Vect p) {
    confVar(float, textSizeLarge);
    text.SetText(str);
    text.SetFont(res.font("bold", textSizeLarge));
    text.SetSize(textSizeLarge);
    text.SetColor(util::to_colour(conf.read<string>("textColour")));
    pos = p;
    align = a;
    valign = v;
}

TextBox::TextBox(): Text(), selColour() {
    selStart = 0;
    selEnd = 0;
}

void TextBox::render() {
    confVar(float, textSize);
    text.SetText(editText);
    sf::FloatRect rect = text.GetRect();
    
    sf::Vector2f clipPos = doAlignment();
    int pixelStart = clipPos.x + text.GetCharacterPos(min(selStart, selEnd)).x;
    int pixelEnd = clipPos.x + text.GetCharacterPos(max(selStart, selEnd)).x + 1;
    sf::Shape box = sf::Shape::Rectangle(pixelStart, clipPos.y, pixelEnd, clipPos.y + rect.GetHeight(), selColour);
    window.Draw(box);
    
    Text::render();
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
