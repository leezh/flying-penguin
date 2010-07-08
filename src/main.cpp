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

#include <iostream>
#include <sstream>
#include <vector>

#define TWEEN_TIME_FUNC = sol::ticks()

#include "sol.hpp"
#include "tween.hpp"
#include "main.hpp"

int size = 20;
sol::limiter fpslimit;
float scale;
namespace res {
    sol::texture *penguin;
    sol::texture *title;
    sol::font *font;
}

inline std::string intToString(int i) {
    std::stringstream out;
    out << i;
    return out.str();
}

inline std::string floatToString(float f, int p = 2) {
    // Resetting the stream is faster than allocating one
    static std::stringstream out;
    out.str("");
    out.setf(std::ios::fixed, std::ios::floatfield);
    out.precision(p);
    out << f;
    return out.str();
}

class appSimulation: public sol::app {
	public:
		sol::rect rectPenguin;
		bool finished;
		bool goUp, goDown;
		bool lastStall;
		unsigned int lastStallTime;

		void reset() {
            lastStall = false;
            lastStallTime = sol::ticks();
            penguin::reset();
            fish::reset();
            particle::reset();
		}

		bool init() {
			rectPenguin = sol::rect(2 * scale, 2 * scale);
			rectPenguin.alignCentre();

            finished = false;
            reset();
			return true;
		}
		void loop() {
			float deltaTime = 1 / fpslimit.tick(100);
			if (penguin::running) {
                if (goDown) penguin::angle -= 0.8 * deltaTime;
                if (goUp) penguin::angle += 0.8 * deltaTime;
			}
			penguin::doPhysics(deltaTime);
			particle::createStar();

			sol::clear2d();
			cloud::render();
			fish::render();
			particle::render();
			glColor4f(1, 1, 1, 1);
			rectPenguin.rotate(-deg(penguin::angle));
			rectPenguin.draw(res::penguin);

			glColor4f(0, 0, 0, 1);
			std::string text = "Distance Covered: " + intToString(penguin::position.i) + "m";
            res::font->draw(text, sol::wm::size().w - res::font->width(text) - 20, 20);
			if (penguin::running) {
                res::font->draw("Height: " + intToString(penguin::position.j) + " m", 20, 20);
                res::font->draw("Air Speed: " + intToString(penguin::airSpeed) + " m/s", 20, 40);

                if (penguin::thrusters) {res::font->draw("Thrusters ON", 20, 60);}
                else {res::font->draw("Thrusters OFF", 20, 60);}

                if (penguin::underspeed) {res::font->draw("Underspeed", 90, 80);}

                if (penguin::stall && sol::ticks() - lastStallTime > 500) {res::font->draw("Stall", 20, 80);}
                if (penguin::stall && !lastStall) {lastStallTime = sol::ticks();}
                lastStall = penguin::stall;

                std::string text = "Fuel: " + intToString(penguin::fuelRemaining) + "s";
                res::font->draw(text, sol::wm::size().w - res::font->width(text) - 20, 40);

			    if (!penguin::takeoff) {
                    res::font->draw("Hold SPACE to use thrusters", 25, sol::wm::size().h - 100);
                    res::font->draw("Press LEFT and RIGHT to steer", 25, sol::wm::size().h - 80);
                    res::font->draw("Collect fish to regain fuel", 25, sol::wm::size().h - 60);
                    res::font->draw("Conserve fuel where necessary", 25, sol::wm::size().h - 40);
			    }
			} else {
			    res::font->draw("Press R to reset", 25, sol::wm::size().h - 40);
			}
            #ifdef DEBUG
			std::string frameRate = intToString(1 / deltaTime) + "FPS";
            res::font->draw(frameRate, sol::wm::size().w - res::font->width(frameRate) - 20,  sol::wm::size().h - 40);
            #endif


			sol::refresh();
		}
		void onKeyboard(int button, bool value)  {
			if (button == SDLK_ESCAPE && !value) {sol::deactivate();}
			if (button == SDLK_q && !value) {sol::deactivate();}
			if (button == SDLK_LEFT) {goUp = value;}
			if (button == SDLK_RIGHT) {goDown = value;}
			if (button == SDLK_SPACE) {penguin::thrusters = value;}
            if (button == SDLK_r && !value) {reset();}
		}
} simulation;

class appTitle: public sol::app {
	public:
		tween *fadeIn, *fadeOut, *slideTo;
		sol::rect rectTitle;
        sol::rect rectPenguin;
		unsigned long int end;
		bool finished;

		bool init() {
			rectTitle = res::title->size();
			rectTitle.alignCentre();
            rectPenguin = sol::rect(2 * scale, 2 * scale);
			rectPenguin.alignCentre();

			fadeIn = new tween_decel(1000, sol::ticks());
			fadeOut = new tween_accel(0, 0);
			slideTo = new tween_smooth(0, 0);
			end = 0;
			finished = false;
			return true;
		}
		void quit() {
			delete fadeIn;
			delete fadeOut;
			delete slideTo;
		}
		void loop() {
			if (finished && sol::ticks() > end) {
			    sol::activate(&simulation); return;
			}
			fpslimit.tick(100);
			sol::clear2d();
			penguin::position.j = 10 - 10 * slideTo->value();
			cloud::render();

			glColor4f(1, 1, 1, 1 - fadeOut->value());
			rectTitle.draw(res::title);

			glColor4f(0, 0, 0, 1 - fadeOut->value());
			std::string str("Press any key to begin");
			int xPos = (sol::wm::size().w - res::font->width(str)) / 2;
			res::font->draw(str, xPos,  sol::wm::size().h - 40);

			glColor4f(.4, .4, .4, 1 - fadeOut->value());
			float dx = res::font->draw("Copyright ", 20, 20);
			dx += res::font->draw(L'©', 20 + dx, 20);
			dx += res::font->draw(" 2010 Lee Zher Huei", 20 + dx, 20);
			res::font->draw("Licensed under GNU GPLv3", 20, 40);

            float groundh = sol::wm::size().h / 2 + penguin::position.j * scale;
			glColor4f(1, 1, 1, 1);
            rectPenguin.y = groundh - rectPenguin.h / 2;
			rectPenguin.rotate(-deg(penguin::angle));
			rectPenguin.draw(res::penguin);

			glColor4f(0, 0, 0, 1 - fadeIn->value());
			sol::wm::size().draw();

			sol::refresh();
		}
		void done() {
		    if (!finished) {
                finished = true;
                end = sol::ticks() + 1500;
                fadeOut->reset(1000, sol::ticks());
                slideTo->reset(1000, sol::ticks());
		    }
		}
		void onMouseButton(int button, bool value) {
			if (button == 1 && value) {done();}
		}
		void onKeyboard(int button, bool value)  {
		    if (button == SDLK_q && !value) {sol::deactivate();}
			if (!value) {done();}
		}
} title;

std::vector <sol::texture> splashImages;
class appSplash: public sol::app {
	public:
		tween *fadeIn, *fadeOut;
		sol::rect rectSplash;
		std::vector <sol::texture>::iterator current;

		bool init() {
			current = splashImages.begin();
			fadeIn = new tween_decel(1000, sol::ticks());
			fadeOut = new tween_accel(1000, sol::ticks() + 2000);
			return true;
		}
		void quit() {
			delete fadeIn;
			delete fadeOut;
		}
		void nextImage() {
		    current++;
		    fadeIn->reset(1000, sol::ticks());
		    fadeOut->reset(1000, sol::ticks() + 2000);
		}
		void loop() {
			if (sol::ticks() > fadeOut->end) {
			    nextImage();
			}
			if (current == splashImages.end()) {
			    sol::activate(&title); return;
			}
			fpslimit.tick(100);
			sol::clear2d();

			glColor4f(1, 1, 1, fadeIn->value() - fadeOut->value());
			rectSplash = current->size();
			rectSplash.alignCentre();
			rectSplash.draw(&(*current));

			sol::refresh();
		}
		void onMouseButton(int button, bool value) {
			if (button == 1 && value) {nextImage();}
		}
		void onKeyboard(int button, bool value)  {
			if (value) {nextImage();}
		}
} splash;

int main(int argc, char *argv[]) {
	if (sol::init(800, 600)) {
	    sol::wm::caption("The Flying Penguin", "art/icon.png");
		scale = sol::wm::size().w / size;

		cloud::init();
		fish::init();
		particle::init();
		res::title = new sol::texture("art/title.png");
		res::penguin = new sol::texture("art/penguin.png");
		res::font = new sol::font("fonts/LiberationSans-Regular.ttf", 16);
		res::font->preloadASCII();
		splashImages.push_back(sol::texture("art/leezh.net.png"));

		sol::activate(&splash);
		sol::run();

		splashImages.clear();
		delete res::title;
		delete res::penguin;
		delete res::font;
		particle::uninit();
		fish::uninit();
		cloud::uninit();

		sol::uninit();
	}
	return 0;
}
