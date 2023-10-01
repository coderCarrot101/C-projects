#!/bin/bash

    gcc -o $1 $2 -I/usr/include/SDL2 -I/usr/include/SDL2/SDL2_ttf-2.20.2 -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_ttf -lm
    x86_64-w64-mingw32-gcc $1_win.c -o $1_win.exe -I/usr/win_SDL/SDL2-2.28.3/x86_64-w64-mingw32/include/SDL2 -I/usr/win_SDL/SDL2_ttf-2.20.2/x86_64-w64-mingw32/include/SDL2 -L/usr/win_SDL/SDL2-2.28.3/x86_64-w64-mingw32/lib -L/usr/win_SDL/SDL2_ttf-2.20.2/x86_64-w64-mingw32/lib -lSDL2 -lSDL2_ttf -lm