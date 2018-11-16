#!/bin/sh

# apt-get install libsdl2-dev libgl1-mesa-glx libgl1-mesa-dri

c++ vmdl.cpp idpo.cpp events.cpp main.cpp -I/usr/include/ -L/usr/lib/ -lSDL2 -lGL -Wpedantic -Wall -Wextra -o view3d # -g
