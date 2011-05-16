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

#include "main.hpp"
#include "record.hpp"

using namespace std;
Record record;

RecordBanner::RecordBanner() {
    text = res.string("large");
}

void RecordBanner::render() {
    Vect pos(window.GetWidth() / 2, window.GetHeight() / 3);
    text->render(pos, "New Record", 0.5f, 0.f);
}

bool RecordBanner::alive() {
    return true;
}

Record::Record() {
    distance = -1;
}

bool Record::submit(float testDist) {
    if (testDist > distance) {
        save.add("record", testDist);
        distance = save.read("record", 0);
        return true;
    }
    return false;
}

float Record::getDist() {
    if (distance < 0) {
        distance = save.read("record", 0);
    }
    return distance;
}
