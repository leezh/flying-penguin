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

#ifndef _LOOPS_HEADER_
#define _LOOPS_HEADER_
#include <stack>

class App {
    public:
        virtual bool init() {return true;}
        virtual void loop() {}
        virtual void quit() {}

        virtual void suspend() {}
        virtual void resume() {}
};

class AppManager {
    private:
        App *activeApp;
        std::stack<App*> suspendedApps;
        
    public:
        void activate(App *a, bool close = true);
        void deactivate();
        void deactivateAll();
        void run();
        AppManager() {activeApp = NULL;};
};

#endif // _LOOPS_HEADER_
