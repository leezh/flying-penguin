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
#include <SFML/Graphics.hpp>
#include "main.hpp"
#include "resources.hpp"
using namespace std;

namespace particle {

sf::Sprite star;

class Particle {
    public:
        float angle;
        float size;
        vect pos;
        sf::Sprite* sprite;
};
vector <Particle> particles;

unsigned int lastCreation = 0;
void createStar() {
    if (!mpenguin.isFlying() || mpenguin.fuel <= 0 || !mpenguin.thrust || lastCreation + 100 > res::clock.GetElapsedTime())
        return;
    lastCreation = res::clock.GetElapsedTime();
    Particle newParticle;
    newParticle.size = (0.2 + 0.3 * rnd()) * scale;
    newParticle.angle = 360 * rnd();
    newParticle.pos = mpenguin.pos;
    newParticle.pos.x += 0.2 * rnd();
    newParticle.pos.y += 0.2 * rnd();
    newParticle.sprite = &star;
    particles.push_back(newParticle);
}

void init() {
    star.SetImage(res::img("star"));
}

void uninit() {
    reset();
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
            if (it->pos.x - mpenguin.pos.x < -20) {
                particles.erase(it);
                redo = true;
                break;
            }
        }
    } while (redo);

    for (it = particles.begin(); it != particles.end(); it++) {
        vect target = (it->pos - mpenguin.pos) * scale;
        target.x += res::window.GetWidth() / 2;
        target.y -= res::window.GetHeight() / 2;
        it->sprite->SetScale(it->size, it->size);
        it->sprite->SetRotation(it->angle);
        it->sprite->SetCenter(res::img("star").GetWidth() / 2, res::img("star").GetHeight() / 2);
        res::window.Draw(*it->sprite);
    }
}

}
