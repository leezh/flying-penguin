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

#ifndef _MAIN_HEADER_
#define _MAIN_HEADER_
#include <cmath>
#include <cstdlib>
#define PI 3.14159265359

#ifdef DEBUG
    #include <iostream>
    #define ERR(x) std::cout << "Error: " << x << std::endl;
    #define LOG(x) std::cout << x << std::endl;
#else
    #define ERR(x)
    #define LOG(X)
#endif

// A vector class that I have created that can handle between one and
// three dimensions. Being completely independent from the rest of the
// code it could be reused in other projects if needed.
class vect {
    public:
        float i, j, k;
        vect(float x = 0, float y = 0, float z = 0): i(x), j(y), k(z) {}

        // Unary operators
        vect operator+() {return vect(i, j, k);}
        vect operator-() {return vect(-i, -j, -k);}

        // Binary operators
        vect operator+(vect v) {return vect(i + v.i, j + v.j, k + v.k);}
        vect operator-(vect v) {return vect(i - v.i, j - v.j, k - v.k);}
        vect operator+=(vect v) {*this = *this + v; return *this;}
        vect operator-=(vect v) {*this = *this - v; return *this;}
        vect operator*(float v) {return vect(i * v, j * v, k * v);}
        vect operator/(float v) {return vect(i / v, j / v, k / v);}

        // Other functions
        float dotProduct(vect v) {return i * v.i + j * v.j + k * v.k;};
        float magnitude() {return sqrt(i * i + j * j + k * k);}
        vect unitVector() {return vect(i, j, k) / magnitude();}
};

// A few oftenly used functions I am too lazy to write all the time
inline float rad(float angle) {
    return angle * PI / 180.;
}

inline float deg(float angle) {
    return angle * 180. / PI;
}

inline float round(float f) {
  return floor(f + 0.5);
}

inline float rnd() {
    return float(rand()) / float(RAND_MAX);
}

namespace penguin {
    extern vect position;
    extern vect velocity;
    extern float angle;
    extern bool thrusters;
    extern float fuelRemaining;

    extern bool running;
    extern bool takeoff;
    extern bool stall;
    extern bool underspeed;
    extern float airSpeed;

    void doPhysics(float deltaTime);
    void reset();
    void launch();
}

// The factor to convert a metre into a pixel.
extern float scale;
extern int size;

namespace cloud {
    void init();
    void uninit();
    void render();
}

namespace fish {
    extern int count;
    extern int missed;

    void init();
    void uninit();
    void doActions();
    void render();
    void reset();
}

namespace particle {
    void init();
    void uninit();
    void reset();
    void render();
    void createStar();
}

#endif // _MAIN_HEADER_
