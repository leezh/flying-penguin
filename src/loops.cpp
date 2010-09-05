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
#include <stack>
#include "loops.hpp"

namespace loops {
    app *activeApp = NULL;
    std::stack<app *> suspendedApps;
    
    void activate(app *a, bool close) {
        if (close) deactivate();
        if (activeApp != NULL) {
            activeApp->suspend();
            suspendedApps.push(activeApp);
        }
        activeApp = a;
        if (!activeApp->init()) deactivate();
    }
    void deactivate() {
        if (activeApp == NULL) return;
        activeApp->quit();
        if (!suspendedApps.empty()) {
            activeApp = suspendedApps.top();
            suspendedApps.pop();
            activeApp->resume();
        } else {
            activeApp = NULL;
        }
    }
    void deactivateAll() {
        if (activeApp == NULL) return;
        activeApp->quit();
        while (!suspendedApps.empty()) {
            activeApp = suspendedApps.top();
            suspendedApps.pop();
            activeApp->quit();
        }
        activeApp = NULL;
    }
    bool run_single() {
        if (activeApp == NULL) return false;
        activeApp->loop();
        return true;
    }
    void run() {
        while (activeApp != NULL) {
            activeApp->loop();
        }
    }
}
