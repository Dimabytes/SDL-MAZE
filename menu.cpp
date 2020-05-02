#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "maze.cpp"

using namespace std;

int showmenu(SDL_Surface *screen, TTF_Font *font) {
    int x, y;
    const int NUMMENU = 2;
    const char *labels[NUMMENU] = {"Generate", "Exit"};
    SDL_Surface *menus[NUMMENU];
    bool selected[NUMMENU] = {false, false};
    SDL_Color color[2] = {{255, 255, 255},
                          {0,   0,   255}};

    menus[0] = TTF_RenderText_Solid(font, labels[0], color[0]);
    menus[1] = TTF_RenderText_Solid(font, labels[1], color[0]);
    SDL_Rect pos[NUMMENU];
    pos[0].x = 30;
    pos[0].y = screen->clip_rect.h / 2 - menus[0]->clip_rect.h;
    pos[1].x = 30;
    pos[1].y = screen->clip_rect.h / 2 + menus[0]->clip_rect.h;

    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    SDL_FreeSurface(menus[0]);
                    SDL_FreeSurface(menus[1]);
                    return 1;

                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    for (int i = 0; i < NUMMENU; i++) {

                        if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                            y >= pos[i].y && y <= pos[i].y + pos[i].h) {
                            if (!selected[i]) {
                                selected[i] = true;
                                SDL_FreeSurface(menus[i]);
                                menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]);
                            }
                        } else if (selected[i]) {
                            selected[i] = false;
                            SDL_FreeSurface(menus[i]);
                            menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
                        }

                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:

                    x = event.button.x;
                    y = event.button.y;
                    for (int i = 0; i < NUMMENU; i += 1) {
                        if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                            y >= pos[i].y && y <= pos[i].y + pos[i].h) {
                            if (i == 0) {
                                draw_maze(screen);
                            } else {
                                SDL_FreeSurface(menus[0]);
                                SDL_FreeSurface(menus[1]);
                                return i;
                            }
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        SDL_FreeSurface(menus[0]);
                        SDL_FreeSurface(menus[1]);
                        return 0;
                    }
            }
        }

        for (int i = 0; i < NUMMENU; i += 1) {
            SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
            SDL_Flip(screen);
        }

    }
}