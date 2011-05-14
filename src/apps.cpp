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
#include <cstddef>
#include "apps.hpp"

void AppManager::activate(App *a, bool close) {
    if (close) deactivate();
    if (activeApp != NULL) {
        activeApp->suspend();
        suspendedApps.push(activeApp);
    }
    activeApp = a;
    if (!activeApp->init()) deactivate();
}
void AppManager::deactivate() {
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
void AppManager::deactivateAll() {
    if (activeApp == NULL) return;
    activeApp->quit();
    while (!suspendedApps.empty()) {
        activeApp = suspendedApps.top();
        suspendedApps.pop();
        activeApp->quit();
    }
    activeApp = NULL;
}
void AppManager::run() {
    while (activeApp != NULL) {
        activeApp->loop();
    }
}

AppManager apps;
