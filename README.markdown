# The Flying Penguin
Trouble struck the penguin community when all the fish in Antarctica started flying out of the water. Using canvas wings and soda-bottle rockets, they quickly took to the skies to collect fish to eat.

In this game, the point is to fly as far as possible while using limited fuel. Extra fuel would be awarded for collecting fish.

# Instructions

     Left - Turn counter-clockwise
    Right - Turn clockwise
    Space - Use thrusters
    Enter - Reset
      Esc - Quit

To take off, hold the `Space` key down to gain speed. When about 13 m/s (~45 km/h) your speed is good enough for take-off. Use the `Left` and `Right` keys to steer.

Be careful of windspeed because there wouldn't be enough lift below 10 m/s. Stall angle is about 25 degrees. At the same time don't hold on to the thrusters for too long as the fuel is limited.

You can actually reach the first few fish without turning off the thrusters, but you'll get trouble later when the fish are further apart. Each time you collect a fish you earn 5 seconds of fuel.

There would be birds flying around that you have to avoid as there would be a 2 second fuel penalty when hit. As you go further, more and more birds would appear.

# License

    The Flying Penguin
    Copyright (C) 2010-2011 Lee Zher Huei <lee.zh.92@gmail.com>
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301, USA.

# Building and Installing

Note: *These instructions are not necessary if you already have a working pre-compiled binary*

Ensure that the following packages and their development files are installed properly:

  * GNU GCC C++
  * GNU Make
  * CMake
  * PhysFS
  * SFML

Run the following commands:

    mkdir build
    cd build
    cmake ..
    make
    make install

# Contributors

* Lee Zher Huei lee@leezh.net
  - Project maintainer
  - Programmer
  - Artist

# External Files

* share/flying-penguin/sounds/buzzer.wav
  - Created by Brandon75689
  - Licensed under Creative Commons Attribution 3.0
  - http://opengameart.org/content/buzzer-wrong-button

* share/flying-penguin/sounds/point-bell.wav
  - Created by Brandon75689
  - Licensed under Creative Commons Attribution 3.0
  - http://opengameart.org/content/point-bell

* share/flying-penguin/music/mushroom-dance.ogg
  - Mushroom Dance
  - Composed by Bart K.
  - Licensed under Creative Commons Attribution 3.0
  - http://opengameart.org/content/mushroom-dance

* src/ConfigFile/ConfigFile.cpp
* src/ConfigFile/ConfigFile.h
  - Configfile
  - Written by Richard J. Wagner
  - Licensed under a zlib-like license
  - http://www-personal.umich.edu/~wagnerr/ConfigFile.html

* share/flying-penguin/fonts/liberation-sans-bold.ttf
* share/flying-penguin/fonts/liberation-sans-regular.ttf
  - Liberation Fonts
  - Licensed under GPLv2 with a font exception
  - https://fedorahosted.org/liberation-fonts/
