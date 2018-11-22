#!/bin/sh

c++ model.cpp idp2.cpp events.cpp main.cpp -I/usr/include/ -L/usr/lib/ -lSDL2 -lGL -Wpedantic -Wall -Wextra -o v3dm # -g
