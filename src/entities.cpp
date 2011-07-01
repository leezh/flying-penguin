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
