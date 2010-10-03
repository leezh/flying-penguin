//  tween.hpp
//  Copyright (C) 2010 Lee Zher Huei <lee@leezh.net>
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
// == About tween.hpp ==
//
// This code was inspiried by Penner's Easing functions, but all the formulas
// were derived on my own because as a maths nerd, I like to do such stuff. ;)
//
// Also, I liked to take advantage of the Object Orientated C++ approach so that
// it could be easily extensible. In fact, the tweening functions could be
// switched in realtime thanks to polymorphism.
//
// == How to use ==
//
// 1. Obtain any time function that suits you. It could represent anything
//    such as seconds, mlliseconds, frames, etc. The only important thing is
//    that you consistently use the same units for all the values that you pass.
//    In these examples we use tf().
//
// 2. To inialise:
//    tween_linear t(42); // Duration of 42 units, but don't start yet
//    tween_linear t(42, tf()); // Duration of 42 units and start now
//    tween_linear t(42, tf() + 6); // Start after 6 units
//
// 3. At the begining of the animation:
// t.reset(tf()); // Start now
// t.reset(tf() + 2); // Start after 2 units
//
// 4. To get some values:
// t.value(tf()) // The position of the function at that moment in time
// 100 + 20 * t.value(tf()) // Starts at 100 and ends at 120
// 100 - 20 * t.value(tf()) // Starts at 100 and ends at 80
// t.done() // Time when the animation ends (in your time function's units)
//
// == How to extend ==
//
// Simply derive the tween class and overload the calculation function with
// your own. The function should plot a graph passing through (0,0) and
// (1,1).

#ifndef _TWEEN_HEADER_
#define _TWEEN_HEADER_

#include <ctime>
#include <cmath>

// Change this if you are not using float
#ifndef TWEEN_UNIT
#define TWEEN_UNIT float
#endif

// Make life easier by defining a default time function before including this
// header file
#ifndef TWEEN_TIME_FUNC
#define TWEEN_TIME_FUNC
#endif

// Does a simple linear calculation.
class tween {
	virtual float calculate(float x) {return x;}

	public:
		TWEEN_UNIT duration, start, end;

		tween(ulint d, ulint s) {reset(d, s);}

		void reset(TWEEN_UNIT duration, TWEEN_UNIT now) {
			this->duration = duration;
			start = now;
			end = start + duration;
		}

		float value(TWEEN_UNIT now TWEEN_TIME_FUNC) {
			if (now < start || duration == 0){
				return 0.f;
			}
			if (now < end && start < end)
				return calculate(float(now - start)/float(duration));
			return 1.f;
		}
};

// This is just to help along with the naming scheme
typedef tween tween_linear;

// Made such that it plots a quad graph with minimum at the origin and passes
// through (1,1)
// So, it is y = x^2
class tween_accel: public tween {
	virtual float calculate(float x) {
		return (x * x);
	}
	public:
		tween_accel(ulint d, ulint s):tween(d, s) {}
};

// Made such that it plots a quad graph with maxium at (1,1) and passes through
// the origin.
// So, it is y = -(x + 1)^2 + 1
//           y = 2x - x^2
class tween_decel: public tween {
	virtual float calculate(float x) {
		return (2 * x) - (x * x);
	}
	public:
		tween_decel(ulint d, ulint s):tween(d, s) {}
};

// I wanted a cubic graph with maxium at (1, 1) and minimum at (1, 1)
// So, it is y = ax^3 + bx^2 + cx
//       dy/dx = 3ax^2 + 2bx + c
// When x = 0 dy/dx = 0
//           0 = c
// When x = 1, dy/dx = 0
//           0 = 3a + 2b
// When x = 1, y = 1
//           0 = a + b - 1
// By solving the two unkowns,
//           y = 3x^2 - 2x^3
class tween_smooth: public tween {
	virtual float calculate(float x) {
		return (3 * x * x) - (2 * x * x * x);
	}
	public:
		tween_smooth(ulint d, ulint s):tween(d, s) {}
};

// A slightly more complicated one. I wanted a cubic graph that passes through
// (0,0) a maxium at (0.5, k) and minimum at (1, 1) where k > 1
// So, it is y = ax^3 + bx^2 + cx
//       dy/dx = 3ax^2 + 2bx + c
// When x = 1, dy/dx = 0
//           0 = 3a + 2b + c
// When x = 0.5, dy/dx = 0
//           0 = 0.75a + b + c
// When x = 1, y = 1
//           1 = a + b + c
// By solving the three unkowns,
//           y = 4x^3 - 9x^2 + 6x
class tween_rebound: public tween {
	virtual float calculate(float x) {
		return (4 * x * x * x) - (9 * x * x) + (6 * x);
	}
	public:
		tween_rebound(ulint d, ulint s):tween(d, s) {}
};

// Based on a circular equation
// y^2 + (x - 1)^2 = 1
//             y^2 = 1 - (x - 1)^2
//             y^2 = 2x - x^2
//               y = sqrt(2x - x^2)
class tween_circular: public tween {
	virtual float calculate(float x) {
		return sqrt((2 * x) - (x * x));
	}
	public:
		tween_circular(ulint d, ulint s):tween(d, s) {}
};

// Based on a circular equation
// (1 - y)^2 + x^2 = 1
//       (1 - y)^2 = 1 - x^2
//           1 - y = sqrt(1 - x^2)
//               y = 1 - sqrt(1 - x^2)
class tween_circular_inv: public tween {
	virtual float calculate(float x) {
		return 1 - sqrt(1 - (x * x));
	}
	public:
		tween_circular_inv(ulint d, ulint s):tween(d, s) {}
};

#endif // _TWEEN_HEADER_
