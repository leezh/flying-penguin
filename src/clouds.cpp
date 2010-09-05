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

namespace cloud {

vector<sf::Sprite> sprites;
sf::Image background;

class Cloud {
    public:
        vect pos;
        int type;
};

class Block {
    public:
        vector<Cloud> clouds;
        vect pos;

        void create(int num, int x, int y) {
            for (int i = 0; i < num; i++) {
                Cloud newCloud;
                newCloud.pos = vect(size * rnd(), size * rnd());
                newCloud.type = sprites.size() * rnd();
                clouds.push_back(newCloud);
            }
            pos = vect(x, y);
        }
        void render(vect cameraPos) {
            vect blockPos = vect(pos.x * size, + pos.y * size);
            vector<Cloud>::iterator it;
            for (it = clouds.begin(); it != clouds.end(); it++) {
                vect target = ((it->pos + blockPos) - cameraPos) * scale;
                target.y = res::window.GetWidth() / 2 - target.y;
                sprites[it->type].SetPosition(target.x, target.y);
                res::window.Draw(sprites[it->type]);
            }
        }
};

vector<Block> blocks;

void init() {
    srand(res::clock.GetElapsedTime());
    sprites.push_back(sf::Sprite(res::img("cloud1")));
    sprites.push_back(sf::Sprite(res::img("cloud2")));
    sprites.push_back(sf::Sprite(res::img("cloud3")));
    background.Create(2, 2, sf::Color(224, 240, 244));
}

void uninit() {
    sprites.clear();
    blocks.clear();
}

void render(vect pos) {
    sf::Sprite backsprite;
    backsprite.SetImage(background);
    backsprite.SetPosition(0, 0);
    backsprite.SetCenter(1, 1);
    backsprite.SetScale(800, 600);
    res::window.Draw(backsprite);
    
    vector<Block> ::iterator it;
    int screenX = floor(pos.x / size);
    int screenY = floor(pos.y / size);
    bool redo = false;
    do {
        redo = false;
        for (it = blocks.begin(); it != blocks.end(); it++) {
            if (abs(it->pos.x - screenX) > 2 && abs(it->pos.y - screenY) > 2) {
                blocks.erase(it);
                redo = true;
                break;
            }
        }
    } while (redo);

    for (int X = screenX - 1; X <= screenX + 1; X++)
        for (int Y = screenY - 1; Y <= screenY + 1; Y++) {
            bool found = false;
            for (it = blocks.begin(); it != blocks.end(); it++) {
                if (it->pos.x == X && it->pos.y == Y) {
                    found = true;
                    it->render(pos);
                    break;
                }
            }
            if (!found) {
                Block newBlock;
                newBlock.create(4, X, Y);
                newBlock.render(pos);
                blocks.push_back(newBlock);
            }
    }
}
}
