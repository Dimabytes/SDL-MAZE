#include "SDL/SDL.h"
#include "SDL/SDL_draw.h"
#include "ctime"
#include <algorithm>

const int WALL_LEN = 20;
const int MAZE_SQUARE = 20;
#define BACK_COLOR 0x00088255

using namespace std;

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

    void drawCell(SDL_Surface *surface) {
        Draw_VLine(surface, x * size + size, y * size, y * size + size, 5000);
        Draw_HLine(surface, x * size, y * size + size, x * size + size, 5000);
        Draw_VLine(surface, x * size, y * size, y * size + size, 5000);
        Draw_HLine(surface, x * size, y * size, x * size + size, 5000);
    }

    void destroyRight(SDL_Surface *surface) {
        hasRight = false;
        Draw_VLine(surface, x * size + size, y * size, y * size + size, BACK_COLOR);
    }

    void destroyBottom(SDL_Surface *surface) {
        hasDown = false;
        Draw_HLine(surface, x * size, y * size + size, x * size + size, BACK_COLOR);
    }

    void destroyLeft(SDL_Surface *surface) {
        hasLeft = false;
        Draw_VLine(surface, x * size, y * size, y * size + size, BACK_COLOR);
    }

    void destroyTop(SDL_Surface *surface) {
        hasUp = false;
        Draw_HLine(surface, x * size, y * size, x * size + size, BACK_COLOR);
    }

};

Cell *newCell(Cell maze[MAZE_SQUARE][MAZE_SQUARE]) {
    for (int i = 0; i < MAZE_SQUARE; i++) {
        for (int j = 0; j < MAZE_SQUARE; j++) {
            if (maze[i][j].visited &&
                ((!(maze[i + 1][j].visited) && i <= MAZE_SQUARE - 2) || (!(maze[i - 1][j].visited) && i >= 1) ||
                 (!(maze[i][j + 1].visited) && j <= MAZE_SQUARE - 2) ||
                 (!(maze[i][j - 1].visited) && j >= 1))) {
                return &maze[i][j];
            }
        }
    }
    return &maze[0][0];
}

void draw_maze(SDL_Surface *screen) {
    SDL_Rect frame_maze;
    frame_maze.x = 150;
    frame_maze.y = 100;
    frame_maze.w = 401;
    frame_maze.h = 401;
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_HWSURFACE |
                                                SDL_DOUBLEBUF, 401, 401, 32,
                                                screen->format->Rmask, screen->format->Gmask,
                                                screen->format->Bmask, screen->format->Amask);
    SDL_FillRect(surface, NULL, BACK_COLOR);
    Cell maze[MAZE_SQUARE][MAZE_SQUARE];
    for (int i = 0; i < MAZE_SQUARE; i++) {
        for (int j = 0; j < MAZE_SQUARE; j++) {
            maze[i][j].x = j;
            maze[i][j].y = i;
            maze[i][j].size = WALL_LEN;
            maze[i][j].drawCell(surface);
        }
    }
    Cell *currentCell = &maze[0][0];
    Cell *compare;
    int nextDirection[4] = {1, 2, 3, 4};
    srand(time(nullptr));
    while (!currentCell->visited) {
        (*currentCell).visited = true;
        random_shuffle(&nextDirection[0], &nextDirection[4]);
        for (int i = 0; i < 4; i++) {
            if (nextDirection[i] == 1 && (*currentCell).hasUp && (*currentCell).y >= 1 &&
                !maze[currentCell->y - 1][currentCell->x].visited) {
                currentCell->destroyTop(surface);
                currentCell = &maze[currentCell->y - 1][currentCell->x];
                (*currentCell).hasDown = false;
                break;
            } else if (nextDirection[i] == 2 && currentCell->hasRight && currentCell->x <= MAZE_SQUARE - 2 &&
                       !maze[currentCell->y][currentCell->x + 1].visited) {
                currentCell->destroyRight(surface);
                currentCell = &maze[(*currentCell).y][(*currentCell).x + 1];
                (*currentCell).hasLeft = false;
                break;
            } else if (nextDirection[i] == 3 && currentCell->hasDown && currentCell->y <= MAZE_SQUARE - 2 &&
                       !maze[currentCell->y + 1][currentCell->x].visited) {
                currentCell->destroyBottom(surface);
                currentCell = &maze[(*currentCell).y + 1][(*currentCell).x];
                (*currentCell).hasUp = false;
                break;
            } else if (nextDirection[i] == 4 && currentCell->hasLeft && currentCell->x >= 1 &&
                       !maze[currentCell->y][currentCell->x - 1].visited) {
                currentCell->destroyLeft(surface);
                currentCell = &maze[(*currentCell).y][(*currentCell).x - 1];
                (*currentCell).hasRight = false;
                break;

            } else if (i == 3) {
                compare = currentCell;
                currentCell = newCell(maze);
                if (compare != currentCell) {
                    (*currentCell).visited = false;
                }
                break;
            }
        }
    }
    SDL_BlitSurface(surface, NULL, screen, &frame_maze);
    SDL_Flip(screen);
}