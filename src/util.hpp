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

#ifndef _UTIL_HEADER_
#define _UTIL_HEADER_

#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>

#define PI 3.14159265359f
class Vect {
    public:
        float x, y, z;
        Vect(float X = 0.f, float Y = 0.f, float Z = 0.f): x(X), y(Y), z(Z) {}

        // Unary operators
        Vect operator+() {return Vect(x, y, z);}
        Vect operator-() {return Vect(-x, -y, -z);}

        // Binary operators
        Vect operator+(const Vect &v) {return Vect(x + v.x, y + v.y, z + v.z);}
        Vect operator-(const Vect &v) {return Vect(x - v.x, y - v.y, z - v.z);}
        Vect operator*(const float &v) {return Vect(x * v, y * v, z * v);}
        Vect operator/(const float &v) {return Vect(x / v, y / v, z / v);}
        
        Vect operator+=(const Vect &v) {*this = *this + v; return *this;}
        Vect operator-=(const Vect &v) {*this = *this - v; return *this;}
        Vect operator*=(const float &v) {*this = *this * v; return *this;}
        Vect operator/=(const float &v) {*this = *this / v; return *this;}
        
        bool operator==(const Vect &v) {if(x == v.x && y == v.y && z == v.z) {return true;} return false;}
        bool operator!=(const Vect &v) {return !(*this == v);}

        // Other functions
        float dotProduct(Vect v) {return x * v.x + y * v.y + z * v.z;};
        float magnitude() {return sqrt(x * x + y * y + z * z);}
        Vect unitVector() {if (magnitude() != 0.f) {return Vect(x, y, z) / magnitude();} return Vect();}
        Vect crossProduct(Vect v) {return Vect(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);};
};

namespace util {

    inline float rad(float angle) {
        return angle * PI / 180.f;
    }

    inline float deg(float angle) {
        return angle * 180.f / PI;
    }

    inline float acuteAngleDiff(const float& a1, const float& a2) {
        float diff = a1 - a2;
        if (diff > PI)
            return diff - 2 * PI;
        if (diff < -PI)
            return 2 * PI - diff;
        return diff;
    }

    inline int round(float f) {
      return (int)floor(f + 0.5f);
    }

    inline float rnd() {
        return float(rand()) / float(RAND_MAX);
    }
    
    inline sf::Color hexToColour(std::string text) {
        unsigned int r = 255, g = 255, b = 255, a = 255;
        
        if (text.size() == 6 || text.size() == 8) {
            std::stringstream ss;
            
            ss << std::hex << text.substr(0, 2);
            ss >> r;
            ss.clear();
            
            ss << std::hex << text.substr(2, 2);
            ss >> g;
            ss.clear();
            
            ss << std::hex << text.substr(4, 2);
            ss >> b;
            ss.clear();
            
            if (text.size() == 8) {
                ss << std::hex << text.substr(6, 2);
                ss >> a;
            }
        }
        return sf::Color(r, g, b, a);
    }
    
    inline std::string floatToString(float f, int p = 2) {
        std::stringstream out;
        out.str("");
        out.setf(std::ios::fixed, std::ios::floatfield);
        out.precision(p);
        out << f;
        return out.str();
    }
    
    inline std::string intToString(int i) {
        std::stringstream out;
        out.str("");
        out << i;
        return out.str();
    }
}

#endif // _UTIL_HEADER_
