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

#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>

//#define TWEEN_TIME_FUNC = sol::ticks()

//#include "tween.hpp"
#include "main.hpp"
#include "resources.hpp"
#include "loops.hpp"

int size = 20;
float scale;
std::string floatToString(float f, int p = 2) {
    // Resetting the stream is faster than allocating one
    static std::stringstream out;
    out.str("");
    out.setf(std::ios::fixed, std::ios::floatfield);
    out.precision(p);
    out << f;
    return out.str();
}
std::string intToString(int i) {
    static std::stringstream out;
    out.str("");
    out << i;
    return out.str();
}

class appSimulation: public loops::app {
	public:
		bool finished;
		void reset() {
            mpenguin.reset();
            //fish::reset();
            particle::reset();
		}
		bool init() {
            finished = false;
            reset();
			return true;
		}
		void loop() {
            sf::Event Event;
            const sf::Input& input = res::window.GetInput();
            while (res::window.GetEvent(Event))
            {
                if (Event.Type == sf::Event::Closed)
                    loops::deactivate();
                if (Event.Type == sf::Event::KeyPressed) {
                    switch (Event.Key.Code) {
                        case sf::Key::Escape:
                        case sf::Key::Q:
                            loops::deactivate();
                            break;
                        case sf::Key::R:
                            reset();
                            break;
                    }
                }
            }
            mpenguin.elevatorAngle = 0;
            if (input.IsKeyDown(sf::Key::Left)) mpenguin.elevatorAngle += rad(10);
            if (input.IsKeyDown(sf::Key::Right)) mpenguin.elevatorAngle -= rad(10);
            mpenguin.thrust = input.IsKeyDown(sf::Key::Space);
			mpenguin.doPhysics(res::window.GetFrameTime());
			particle::createStar();

			res::window.Clear();
			cloud::render(mpenguin.pos);
			//fish::render();
			particle::render();
            mpenguin.render();

			/*glColor4f(0, 0, 0, 1);
			std::string text = "Distance Covered: " + intToString(mpenguin.pos.i) + "m";
            res::font->draw(text, sol::wm::size().w - res::font->width(text) - 20, 20);
			if (mpenguin.isAlive()) {
			    std::string text;
                res::font->draw("Height: " + intToString(mpenguin.pos.j) + " m", 20, 20);
                res::font->draw("Air Speed: " + intToString(mpenguin.windSpeed()) + " m/s", 20, 40);

                if (mpenguin.isUnderspeed()) {
                    glColor4f(0.8, 0, 0, 1);
                    text = "Underspeed";
                    int x = (sol::wm::size().w - res::font->width(text)) / 2;
                    int y = sol::wm::size().h / 2 - 6 * scale + 20;
                    res::font->draw(text, x, y);
                    glColor4f(0, 0, 0, 1);
                }

                if (mpenguin.isStalling()) {
                    glColor4f(0.8, 0, 0, 1);
                    text = "Stall";
                    int x = (sol::wm::size().w - res::font->width(text)) / 2;
                    int y = sol::wm::size().h / 2 - 6 * scale;
                    res::font->draw(text, x, y);
                    glColor4f(0, 0, 0, 1);
                }

                text = "Fuel: " + intToString(penguin::fuelRemaining) + "s";
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
            #ifdef DEBUG_MSG
			std::string frameRate = intToString(1 / deltaTime) + "FPS";
            res::font->draw(frameRate, sol::wm::size().w - res::font->width(frameRate) - 20,  sol::wm::size().h - 40);
            #endif*/
            
			res::window.Display();
		}
} simulation;
/*
class appTitle: public sol::app {
	public:
		tween *fadeIn, *fadeOut, *slideTo;
		sf::Sprite title;
		unsigned long int end;
		bool finished;

		bool init() {
			rectTitle = res::title->size();
			rectTitle.alignCentre();
            rectPenguin = sol::rect(2.5 * scale, 2.5 * scale);
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
            float cameraHeight = 10 - 10 * slideTo->value();
			cloud::render(vect(0, cameraHeight));

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

            float groundh = sol::wm::size().h / 2 + cameraHeight * scale;
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
*/
int main(int argc, char *argv[]) {
        res::window.Create(sf::VideoMode(800, 600, 32), "The Flying Penguin");
        res::window.UseVerticalSync(true);
		scale = res::window.GetWidth() / size;

		cloud::init();
		//fish::init();
		particle::init();

		loops::activate(&simulation);
		loops::run();

		//splashImages.clear();
		particle::uninit();
		//fish::uninit();
		cloud::uninit();
	return 0;
}
