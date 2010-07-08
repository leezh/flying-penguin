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

#include "sol.hpp"
#include "main.hpp"
using namespace sol;
using namespace std;

namespace fish {
texture *fishTex, *arrow;
int missed = 0;
int count = 0;
vect position;

void init() {
    fishTex = new sol::texture("art/fish.png");
    arrow = new sol::texture("art/arrow.png");
}

void uninit() {
    delete fishTex;
    delete arrow;
}

void reposition () {
    // Oh, yes it gets tougher as you go further
    // Eventually there would be a point where it
    // is no longer possible to collect it anymore
    position.i = penguin::position.i * 1.1+ 120;
    position.j = 10 + 50 * rnd();
}

void render() {
    if (position.i - penguin::position.i < -20) {
        missed++;
        reposition();
    }

    vect vtarget = position - penguin::position;
    if (vtarget.magnitude() < 3) {
        count++;
        reposition();
        vtarget = position - penguin::position;
        penguin::fuelRemaining += 2.5;
    }

    glColor4f(1, 1, 1, 1);
    sol::rect target = sol::rect(2 * scale, 2 * scale);
    target.x = wm::size().w / 2 + vtarget.i * scale - fishTex->w / 2;
    target.y = wm::size().h / 2 - vtarget.j * scale - fishTex->h / 2;
    target.draw(fishTex);

    if (vtarget.magnitude() > 5) {
        target = rect(1.6 * scale, 0.4 * scale);
        target.alignCentre();
        target.x += target.w * 2;
        target.rotate(-deg(atan2(vtarget.j, vtarget.i)), -2);
        target.draw(arrow);
    }
}

void reset() {
    count = 0;
    missed = 0;
    reposition();
    // Give enough distance for take-off
    position.i = penguin::position.i + 200;
}
}
