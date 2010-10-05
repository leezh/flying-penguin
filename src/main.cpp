//  The Flying Penguin
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
            penguin.reset();
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
            penguin.elevatorAngle = 0;
            if (input.IsKeyDown(sf::Key::Left)) penguin.elevatorAngle += rad(20);
            if (input.IsKeyDown(sf::Key::Right)) penguin.elevatorAngle -= rad(20);
            penguin.thrust = input.IsKeyDown(sf::Key::Space);
            penguin.doPhysics(res::window.GetFrameTime());
            particle::createStar();

            res::window.Clear();
            cloud::render(penguin.pos);
            //fish::render();
            particle::render();
            penguin.render();
            
            sf::String str;
            str.SetFont(res::fnt("regular", 18));
            str.SetSize(18);
            str.SetColor(sf::Color(0, 0 ,0));

            str.SetText(intToString(round(1 / res::window.GetFrameTime())) + "FPS");
            str.SetCenter(str.GetRect().GetWidth(), 0);
            str.SetPosition((float)res::window.GetWidth() - 20, (float)res::window.GetHeight() - 40);
            res::window.Draw(str);
            
            if (penguin.isFlying()) {
                str.SetText("Distance Covered: " + intToString(round(penguin.flightDist())) + "m");
                str.SetCenter(str.GetRect().GetWidth(), 0);
                str.SetPosition((float)res::window.GetWidth() - 20, 20);
                res::window.Draw(str);

                if (penguin.isAlive()) {
                    str.SetText("Fuel: " + intToString(round(penguin.fuel)) + "s");
                    str.SetCenter(str.GetRect().GetWidth(), 0);
                    str.SetPosition((float)res::window.GetWidth() - 20, 40);
                    res::window.Draw(str);
                }
            } else {
                str.SetCenter(0, 0);
                str.SetText("Hold SPACE to use thrusters");
                str.SetPosition(20, (float)res::window.GetHeight() - 100);
                res::window.Draw(str);

                str.SetText("Press LEFT and RIGHT to steer");
                str.SetPosition(20, (float)res::window.GetHeight() - 80);
                res::window.Draw(str);

                str.SetText("Collect fish to regain fuel");
                str.SetPosition(20, (float)res::window.GetHeight() - 60);
                res::window.Draw(str);

                str.SetText("Conserve fuel where necessary");
                str.SetPosition(20, (float)res::window.GetHeight() - 40);
                res::window.Draw(str);
            }
            
            if (penguin.isAlive()) {
                str.SetCenter(0, 0);
                
                str.SetText("Height: " + intToString(round(penguin.pos.y)) + " m");
                str.SetPosition(20, 20);
                res::window.Draw(str);
                
                str.SetText("Air Speed: " + intToString(round(penguin.windSpeed())) + " m/s");
                str.SetPosition(20, 40);
                res::window.Draw(str);
            } else {
                str.SetText("Press R to reset");
                str.SetCenter(0, 0);
                str.SetPosition(20, (float)res::window.GetHeight() - 40);
                res::window.Draw(str);
            }
            
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
    sf::Image *icon = &res::img("icon");
    res::window.Create(sf::VideoMode(800, 600, 32), "The Flying Penguin");
    res::window.UseVerticalSync(true);
    res::window.SetIcon(icon->GetWidth(), icon->GetHeight(), icon->GetPixelsPtr());
    scale = (float)res::window.GetWidth() / size;

    cloud::init();
    //fish::init();
    particle::init();
    penguin.init();

    loops::activate(&simulation);
    loops::run();

    //splashImages.clear();
    particle::uninit();
    //fish::uninit();
    cloud::uninit();
    return 0;
}
