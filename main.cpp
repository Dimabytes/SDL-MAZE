#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "menu.cpp"

#define BACK_COLOR 0x00088255
const int S_WIDTH = 600;
const int S_HEIGHT = 600;

using namespace std;

int main(int argc, char *argv[]) {
    SDL_Surface *screen;
    SDL_Init(SDL_INIT_EVERYTHING);
    screen = SDL_SetVideoMode(S_WIDTH, S_HEIGHT, 32, SDL_HWSURFACE);
    SDL_FillRect(screen, NULL, BACK_COLOR);
    SDL_Flip(screen);
    if (!screen) {
        cout << "SDL FAIL";
        atexit(SDL_Quit);
        return 3;
    }
    TTF_Init();
    TTF_Font *font;
    font = TTF_OpenFont("font.ttf", 16);
    showmenu(screen, font);
    return 0;
}

