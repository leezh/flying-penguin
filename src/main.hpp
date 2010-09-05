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
#include <string>
#include <SFML/Graphics.hpp>

#define PI 3.14159265359
#include "config.h"

#ifdef DEBUG_MSG
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
        float x, y, z;
        vect(float X = 0, float Y = 0, float Z = 0): x(X), y(Y), z(Z) {}

        // Unary operators
        vect operator+() {return vect(x, y, z);}
        vect operator-() {return vect(-x, -y, -z);}

        // Binary operators
        vect operator+(const vect &v) {return vect(x + v.x, y + v.y, z + v.z);}
        vect operator-(const vect &v) {return vect(x - v.x, y - v.y, z - v.z);}
        vect operator*(const float &v) {return vect(x * v, y * v, z * v);}
        vect operator/(const float &v) {return vect(x / v, y / v, z / v);}
        
        vect operator+=(const vect &v) {*this = *this + v; return *this;}
        vect operator-=(const vect &v) {*this = *this - v; return *this;}
        vect operator*=(const float &v) {*this = *this * v; return *this;}
        vect operator/=(const float &v) {*this = *this / v; return *this;}
        
        bool operator==(const vect &v) {if(x == v.x && y == v.y && z == v.z) {return true;} return false;}
        bool operator!=(const vect &v) {return !(*this == v);}

        // Other functions
        float dotProduct(vect v) {return x * v.x + y * v.y + z * v.z;};
        float magnitude() {return sqrt(x * x + y * y + z * z);}
        vect unitVector() {if (magnitude() != 0.f) {return vect(x, y, z) / magnitude();} return vect();}
        vect crossProduct(vect v) {return vect(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);};
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
    extern bool crashed;
    extern float airSpeed;
    extern float elevatorAngle;

    void doPhysics(float deltaTime);
    void reset();
    void launch();
}

class Penguin {
    private:
        bool running;
        bool takeoff;
        sf::Sprite sprite;
        
    public:
        vect pos;
        vect vel;
        float angle;
        bool thrust;
        float fuel;
        float elevatorAngle;
        
        Penguin();
        
        bool isAlive();
        bool isFlying();
        bool isUnderspeed();
        bool isStalling();
        float windSpeed();
        float windAngle();
        
        void reset();
        void doPhysics(float deltaTime);
        void render();
};

extern Penguin mpenguin;

// The factor to convert a metre into a pixel.
extern float scale;
extern int size;

namespace cloud {
    void init();
    void uninit();
    void render(vect pos);
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
