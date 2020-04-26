#include <stdio.h>
#include <cstdlib>
#include<ctime>
#include<iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_draw.h>
#include <algorithm>

const int WALL_LEN = 64;


class Cell {

public:
    int x;
    int y;
    int size;
    bool visited;
    bool hasLeft;
    bool hasRight;
    bool hasDown;
    bool hasUp;

    Cell() {
        this->x = 0;
        this->y = 0;
        this->visited = false;
        this->hasLeft = true;
        this->hasRight = true;
        this->hasDown = true;
        this->hasUp = true;
    }

    void drawCell(SDL_Surface *surface){
        SDL_Rect square;
        square.h = WALL_LEN;
        square.w = WALL_LEN;
        square.x = this->x * WALL_LEN;
        square.y = this->y * WALL_LEN;
        SDL_FillRect(surface, &square, 0);
    }

    void destroyRight(SDL_Surface *surface){
        this->hasRight = false;
        Draw_VLine(surface, this->x * WALL_LEN + WALL_LEN, this->y *  WALL_LEN, this->y * WALL_LEN + WALL_LEN, 5000);
    }

    void destroyBottom(SDL_Surface *surface){
        this->hasDown = false;
        Draw_HLine(surface, this->x * WALL_LEN + WALL_LEN, this->y *  WALL_LEN, this->x * WALL_LEN + WALL_LEN, 5000);
    }

    void destroyLeft(SDL_Surface *surface){
        this->hasLeft = false;
        Draw_VLine(surface, this->x * WALL_LEN, this->y *  WALL_LEN, this->y * WALL_LEN + WALL_LEN, 5000);
    }

    void destroyTop(SDL_Surface *surface){
        this->hasUp = false;
        Draw_HLine(surface, this->x * WALL_LEN, this->y *  WALL_LEN, this->x * WALL_LEN + WALL_LEN, 5000);
    }

};

using namespace std;

Cell *backToUnvisitedCell(Cell grid[10][10]);

int main(int argc, char *argv[]) {
    Cell grid[10][10];
    SDL_Surface *screen;
    SDL_Init(SDL_INIT_EVERYTHING);
    screen = SDL_SetVideoMode(640, 640, 32, SDL_HWSURFACE);
    if (!screen) {
        cout << "SDL MODE FAILED: %s\n" << SDL_GetError();
        atexit(SDL_Quit);
        return 3;
    }
    SDL_FillRect(screen, NULL, 0xffffff);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF)); //white background
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j].x = j;
            grid[i][j].y = i;
            grid[i][j].drawCell(screen);
        }
    }
    SDL_Rect verticalLine;
    verticalLine.h = WALL_LEN;
    verticalLine.w = 1;
    SDL_Rect horizontalLine;
    horizontalLine.h = 1;
    horizontalLine.w = WALL_LEN;
    SDL_Flip(screen);
    Cell *currentCell = &grid[0][0];
    Cell *compare;
    int nextDirection[4] = {1, 2, 3, 4};
    srand(time(0)); //we use this to override the default random shuffle seed
    random_shuffle(&nextDirection[0], &nextDirection[3]);

    while (!currentCell->visited) {
        (*currentCell).visited = true;
        random_shuffle(&nextDirection[0], &nextDirection[4]);
        for (int i = 0; i < 4; i++) {
            if (nextDirection[i] == 1 && (*currentCell).hasUp && (*currentCell).y >= 1 &&
                !grid[currentCell->y - 1][currentCell->x].visited) {
                currentCell->destroyTop(screen);

                currentCell = &grid[currentCell->y - 1][currentCell->x];
                (*currentCell).hasDown = false;
                break;
            } else if (nextDirection[i] == 2 && currentCell->hasRight && currentCell->x <= 8 &&
                       !grid[currentCell->y][currentCell->x + 1].visited) {
                currentCell->destroyRight(screen);

                currentCell = &grid[(*currentCell).y][(*currentCell).x + 1];
                (*currentCell).hasLeft = false;
                break;
            } else if (nextDirection[i] == 3 && currentCell->hasDown && currentCell->y <= 8 &&
                       !grid[currentCell->y + 1][currentCell->x].visited) {

                currentCell->destroyBottom(screen);

                currentCell = &grid[(*currentCell).y + 1][(*currentCell).x];
                (*currentCell).hasUp = false;
                break;
            } else if (nextDirection[i] == 4 && currentCell->hasLeft && currentCell->x >= 1 &&
                       !grid[currentCell->y][currentCell->x - 1].visited) {

                currentCell->destroyLeft(screen);

                currentCell = &grid[(*currentCell).y][(*currentCell).x - 1];
                (*currentCell).hasRight = false;
                break;

            } else if (i == 3) {
                compare = currentCell;
                currentCell = backToUnvisitedCell(grid);
                if (compare != currentCell) {
                    (*currentCell).visited = false;
                }
                break;
            }

            SDL_Flip(screen);
        }
    }

    // The window is open: could enter program loop here (see SDL_PollEvent())

    SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window

    // Clean up
    SDL_Quit();
    return 0;
}

Cell *backToUnvisitedCell(Cell grid[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j].visited && ((!(grid[i + 1][j].visited) && i <= 8) || (!(grid[i - 1][j].visited) && i >= 1) ||
                                       (!(grid[i][j + 1].visited) && j <= 8) ||
                                       (!(grid[i][j - 1].visited) && j >= 1))) {
                //we still have areas to explore here
                return &grid[i][j];
            }
        }
    }
    //returning the first visited Cell
    return &grid[0][0];
}