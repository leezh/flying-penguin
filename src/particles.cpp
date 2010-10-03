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
float starInterval = 0.1f; // Delay between stars created
float starJitter = 0.2f; // Max random offset in position
float starLifespan = 0.8f; // Lifespan of one star in seconds
float starSizeMax = 0.7f; // Maxium size of the star
float starSizeMin = 0.5f; // Minimum size of the star

class Particle {
    public:
        float angle;
        float size;
        float birthTime;
        float lifespan;
        vect pos;
        sf::Sprite* sprite;
};
vector <Particle> particles;

float lastCreation = 0;
void createStar() {
    if (!penguin.isAlive() || penguin.fuel <= 0 || !penguin.thrust)
        return;
    if (lastCreation + starInterval > res::clock.GetElapsedTime())
        return;
    lastCreation = res::clock.GetElapsedTime();
    Particle newParticle;
    newParticle.size = (starSizeMin + (starSizeMax - starSizeMin) * rnd());
    newParticle.angle = 360 * rnd();
    newParticle.pos = penguin.pos + vect(starJitter, starJitter) * rnd();
    newParticle.sprite = &star;
    newParticle.birthTime = res::clock.GetElapsedTime();
    newParticle.lifespan = starLifespan;
    particles.push_back(newParticle);
}

void init() {
    star.SetImage(res::img("star"));
    star.SetCenter(res::img("star").GetWidth() / 2.f, res::img("star").GetHeight() / 2.f);
}

void uninit() {
    reset();
}

void reset() {
    particles.clear();
}

void render() {
    vector<Particle>::iterator it;
    float now = res::clock.GetElapsedTime();

    bool redo = false;
    do {
        redo = false;
        for (it = particles.begin(); it != particles.end(); it++) {
            if (it->birthTime + it->lifespan < now) {
                particles.erase(it);
                redo = true;
                break;
            }
        }
    } while (redo);

    for (it = particles.begin(); it != particles.end(); it++) {
        vect target = (it->pos - penguin.pos) * scale;
        target.x += res::window.GetWidth() / 2;
        target.y = res::window.GetHeight() / 2 - target.y;
        float fadeFactor = 1 - (now - it->birthTime) / it->lifespan;
        it->sprite->SetScale(it->size, it->size);
        it->sprite->SetPosition(target.x, target.y);
        it->sprite->SetRotation(it->angle);
        it->sprite->SetColor(sf::Color(255, 255, 255, int(255 * fadeFactor)));
        res::window.Draw(*it->sprite);
    }
}

}
