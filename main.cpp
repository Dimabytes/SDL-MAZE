#include <stdio.h>
#include <cstdlib>
#include<ctime>
#include<iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_draw.h>
#include <algorithm>


const int S_WIDTH = 600;
const int S_HEIGHT = 600;
const int WALL_LEN = 16;
const int MAZE_SQUARE = 30;

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
        this->size = 0;
        this->visited = false;
        this->hasLeft = true;
        this->hasRight = true;
        this->hasDown = true;
        this->hasUp = true;
    }

    void drawCell(SDL_Surface *surface){
        Draw_VLine(surface, this->x * this->size + this->size, this->y *  this->size, this->y * this->size + this->size, 5000);
        Draw_HLine(surface, this->x * this->size, this->y * this->size + this->size, this->x * this->size + this->size, 5000);
        Draw_VLine(surface, this->x * this->size, this->y *  this->size, this->y * this->size + this->size, 5000);
        Draw_HLine(surface, this->x * this->size, this->y *  this->size, this->x * this->size + this->size, 5000);
    }

    void destroyRight(SDL_Surface *surface){
        this->hasRight = false;
        Draw_VLine(surface, this->x * this->size + this->size, this->y *  this->size, this->y * this->size + this->size, 0);
    }

    void destroyBottom(SDL_Surface *surface){
        this->hasDown = false;
        Draw_HLine(surface, this->x * this->size, this->y * this->size + this->size, this->x * this->size + this->size, 0);
    }

    void destroyLeft(SDL_Surface *surface){
        this->hasLeft = false;
        Draw_VLine(surface, this->x * this->size, this->y *  this->size, this->y * this->size + this->size, 0);
    }

    void destroyTop(SDL_Surface *surface){
        this->hasUp = false;
        Draw_HLine(surface, this->x * this->size, this->y *  this->size, this->x * this->size + this->size, 0);
    }

};

using namespace std;

Cell *backToUnvisitedCell(Cell maze[MAZE_SQUARE][MAZE_SQUARE]);

int main(int argc, char *argv[]) {
    Cell maze[MAZE_SQUARE][MAZE_SQUARE];
    SDL_Surface *screen;
    SDL_Init(SDL_INIT_EVERYTHING);
    screen = SDL_SetVideoMode(S_WIDTH, S_HEIGHT, 32, SDL_HWSURFACE);
    if (!screen) {
        cout << "SDL FAIL";
        atexit(SDL_Quit);
        return 3;
    }
    SDL_FillRect(screen, NULL, 0);
    for (int i = 0; i < MAZE_SQUARE; i++) {
        for (int j = 0; j < MAZE_SQUARE; j++) {
            maze[i][j].x = j;
            maze[i][j].y = i;
            maze[i][j].size = WALL_LEN;
            maze[i][j].drawCell(screen);
        }
    }
    Cell *currentCell = &maze[0][0];
    Cell *compare;
    int nextDirection[4] = {1, 2, 3, 4};
    srand(time(0));
    while (!currentCell->visited) {
        (*currentCell).visited = true;
        random_shuffle(&nextDirection[0], &nextDirection[4]);
        for (int i = 0; i < 4; i++) {
            if (nextDirection[i] == 1 && (*currentCell).hasUp && (*currentCell).y >= 1 &&
                !maze[currentCell->y - 1][currentCell->x].visited) {
                currentCell->destroyTop(screen);
                currentCell = &maze[currentCell->y - 1][currentCell->x];
                (*currentCell).hasDown = false;
                break;
            } else if (nextDirection[i] == 2 && currentCell->hasRight && currentCell->x <= MAZE_SQUARE - 2 &&
                       !maze[currentCell->y][currentCell->x + 1].visited) {
                currentCell->destroyRight(screen);
                currentCell = &maze[(*currentCell).y][(*currentCell).x + 1];
                (*currentCell).hasLeft = false;
                break;
            } else if (nextDirection[i] == 3 && currentCell->hasDown && currentCell->y <= MAZE_SQUARE - 2 &&
                       !maze[currentCell->y + 1][currentCell->x].visited) {
                currentCell->destroyBottom(screen);
                currentCell = &maze[(*currentCell).y + 1][(*currentCell).x];
                (*currentCell).hasUp = false;
                break;
            } else if (nextDirection[i] == 4 && currentCell->hasLeft && currentCell->x >= 1 &&
                       !maze[currentCell->y][currentCell->x - 1].visited) {
                currentCell->destroyLeft(screen);
                currentCell = &maze[(*currentCell).y][(*currentCell).x - 1];
                (*currentCell).hasRight = false;
                break;

            } else if (i == 3) {
                compare = currentCell;
                currentCell = backToUnvisitedCell(maze);
                if (compare != currentCell) {
                    (*currentCell).visited = false;
                }
                break;
            }

            SDL_Flip(screen);
        }
    }

    SDL_Event event;
    while(SDL_WaitEvent(&event)){
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
            atexit(SDL_Quit);
            return 0;
        }
    }
    return 0;
}

Cell *backToUnvisitedCell(Cell maze[MAZE_SQUARE][MAZE_SQUARE]) {
    for (int i = 0; i < MAZE_SQUARE; i++) {
        for (int j = 0; j < MAZE_SQUARE; j++) {
            if (maze[i][j].visited && ((!(maze[i + 1][j].visited) && i <= MAZE_SQUARE - 2) || (!(maze[i - 1][j].visited) && i >= 1) ||
                                       (!(maze[i][j + 1].visited) && j <= MAZE_SQUARE - 2) ||
                                       (!(maze[i][j - 1].visited) && j >= 1))) {
                return &maze[i][j];
            }
        }
    }
    //returning the first visited Cell
    return &maze[0][0];
}