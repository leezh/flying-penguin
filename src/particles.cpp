//  The Flying Penguin
//  Copyright (C) 2010 Lee Zher Huei <lee@leezh.net>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
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

#include <vector>
#include "sol.hpp"
#include "main.hpp"
using namespace penguin;
using namespace sol;
using namespace std;

namespace particle {

texture *star;
texture *flash;

class Particle {
    public:
        float angle;
        int size;
        vect pos;
        texture *tex;
};
vector <Particle> particles;

unsigned int lastCreation = 0;
void createStar() {
    if (!running || fuelRemaining <= 0 || !thrusters || lastCreation + 100 > sol::ticks())
        return;
    lastCreation = sol::ticks();
    Particle newParticle;
    newParticle.size = (0.2 + 0.3 * rnd()) * scale;
    newParticle.angle = 360 * rnd();
    newParticle.pos = penguin::position;
    newParticle.pos.i += 0.2 * rnd();
    newParticle.pos.j += 0.2 * rnd();
    newParticle.tex = star;
    particles.push_back(newParticle);
}

void init() {
    star = new texture("art/star.png");
}

void uninit() {
    delete star;
    particles.clear();
}

void reset() {
    particles.clear();
}

void render() {
    vector<Particle>::iterator it;

    bool redo = false;
    do {
        redo = false;
        for (it = particles.begin(); it != particles.end(); it++) {
            if (it->pos.i - penguin::position.i < -20) {
                particles.erase(it);
                redo = true;
                break;
            }
        }
    } while (redo);
    glColor4f(1, 1, 1, 1);

    for (it = particles.begin(); it != particles.end(); it++) {
        vect vtarget = it->pos - penguin::position;
        sol::rect target = rect(it->size, it->size);
        target.x = wm::size().w / 2 + vtarget.i * scale - target.w / 2;
        target.y = wm::size().h / 2 - vtarget.j * scale - target.h / 2;
        target.rotate(it->angle);
        target.draw(it->tex);
    }
}

}
