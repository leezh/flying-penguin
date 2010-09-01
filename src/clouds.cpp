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
using namespace sol;
using namespace std;

namespace cloud {

vector<texture> textures;

class Cloud {
    public:
        vect pos;
        int type;
};

class Block {
    public:
        vector<Cloud> clouds;
        int posX, posY;

        void create(int num, int x, int y) {
            for (int i = 0; i < num; i++) {
                Cloud newCloud;
                newCloud.pos = vect(size * rnd(), size * rnd());
                newCloud.type = textures.size() * rnd();
                clouds.push_back(newCloud);
            }
            posX = x;
            posY = y;
        }
        bool inRange(int x, int y) {
            if (abs(posX - x) < 2 && abs(posY - y) < 2) return true;
            return false;
        }

        void render(vect p) {
            vect pos = vect(posX * size, + posY * size);
            vector<Cloud>::iterator it;
            for (it = clouds.begin(); it != clouds.end(); it++) {
                vect vtarget = (it->pos + pos) - p;
                sol::rect target = textures[it->type].size();
                target.x = vtarget.i * scale;
                target.y = wm::size().h / 2 - vtarget.j * scale;
                target.draw(&textures[it->type]);
            }
        }
};

vector<Block> blocks;

void init() {
    srand(sol::ticks());
    textures.push_back(texture(resDir + "images/cloud1.png"));
    textures.push_back(texture(resDir + "images/cloud2.png"));
    textures.push_back(texture(resDir + "images/cloud3.png"));
}

void uninit() {
    textures.clear();
    blocks.clear();
}

void render(vect pos) {
    int w = wm::size().w;
    int h = wm::size().h;
    glBegin(GL_QUADS);
    glColor4f(181./256., 205./256., 202./256., 1);
    glVertex2i(0, 0); glVertex2i(w, 0);
    glColor4f(234./256., 241./246., 241./246., 1);
    glVertex2i(w, h); glVertex2i(0, h);
    glEnd();
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    float groundh = h / 2 + pos.j * scale + 0.4 * scale;
    glVertex2i(0, groundh); glVertex2i(w, groundh);
    glVertex2i(w, h); glVertex2i(0, h);
    glEnd();
    glColor4f(1, 1, 1, 1);

    vector<Block> ::iterator it;
    // Current X and Y
    int cx = floor(pos.i / size);
    int cy = floor(pos.j / size);
    bool redo = false;
    do {
        redo = false;
        for (it = blocks.begin(); it != blocks.end(); it++) {
            if (!it->inRange(cx, cy)) {
                blocks.erase(it);
                redo = true;
                break;
            }
        }
    } while (redo);

    for (int X = cx - 1; X <= cx + 1; X++)
        for (int Y = cy - 1; Y <= cy + 1; Y++) {
            bool found = false;
            for (it = blocks.begin(); it != blocks.end(); it++) {
                if (it->posX == X && it->posY == Y) {
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
