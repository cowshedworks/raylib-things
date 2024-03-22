#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

#define ALIVE 1
#define DEAD 0

typedef struct Screen
{
    int height;
    int width;
    int top;
    int bottom;
    int left;
    int right;
    Color color;
} Screen;

typedef struct Automata
{
    int posX;
    int posY;
    int height;
    int width;
    int state;
    Color color;
} Automata;

const Screen screen = {1200, 2000, 0, 450, 0, 800, BLACK};
const int automataSize = 3;
const int defaultState = 1;
const int fps = 5;
bool paused = false;

int getAutomataNextState(Automata automata, Automata board[screen.width][screen.height])
{
    // in here we grab the next state of the automata
    // 1 ALIVE, 2 DEAD
    int topNeighbour;
    int topRightNeighbour;
    int rightNeighbour;
    int bottomRightNeighbour;
    int bottomNeighbour;
    int bottomLeftNeighbour;
    int leftNeighbour;
    int topLeftNeighbour;

    // compute neighbour states, we're not handling screen wrapping so
    // off-board neighbours are dead to us
    topNeighbour = (automata.posY - automataSize <= 0) ? 0 : board[automata.posX][automata.posY - automataSize].state;
    topRightNeighbour = ((automata.posY - automataSize) <= 0 || (automata.posX + automataSize) > screen.width) ? 0 : board[automata.posX + automataSize][automata.posY - automataSize].state;
    rightNeighbour = ((automata.posX + automataSize) > screen.width) ? 0 : board[automata.posX + automataSize][automata.posY].state;
    bottomRightNeighbour = ((automata.posX + automataSize) > screen.width || (automata.posY + automataSize) > screen.height) ? 0 : board[automata.posX + automataSize][automata.posY + automataSize].state;
    bottomNeighbour = ((automata.posY + automataSize) > screen.height) ? 0 : board[automata.posX][automata.posY + automataSize].state;
    bottomLeftNeighbour = ((automata.posX - automataSize) <= 0 || (automata.posY + automataSize) > screen.height) ? 0 : board[automata.posX - automataSize][automata.posY + automataSize].state;
    leftNeighbour = ((automata.posX - automataSize) <= 0) ? 0 : board[automata.posX - automataSize][automata.posY].state;
    topLeftNeighbour = ((automata.posY - automataSize) <= 0 || (automata.posX - automataSize) >= 0) ? 0 : board[automata.posX - automataSize][automata.posY - automataSize].state;

    int aliveNeighbours = topNeighbour + topRightNeighbour + rightNeighbour + bottomRightNeighbour + bottomNeighbour + bottomLeftNeighbour + leftNeighbour + topLeftNeighbour;

    if (automata.state == DEAD && aliveNeighbours == 3)
    {
        return ALIVE;
    }

    if (automata.state == ALIVE && (aliveNeighbours < 2 || aliveNeighbours > 3))
    {
        return DEAD;
    }

    if (automata.state == ALIVE && (aliveNeighbours == 2 || aliveNeighbours == 3))
    {
        return ALIVE;
    }

    return automata.state;
}

void togglePause()
{
    if (paused)
    {
        paused = false;
    }
    else
    {
        paused = true;
    }
}

void drawBoard(Automata automata[screen.width][screen.height])
{
    int x, y;
    for (x = 1; x < screen.width; x = x + automataSize)
    {
        for (y = 1; y < screen.height; y = y + automataSize)
        {
            DrawRectangle(
                automata[x][y].posX,
                automata[x][y].posY,
                automata[x][y].height,
                automata[x][y].width,
                automata[x][y].color);
        }
    }
}

void computeNextBoard(Automata writeBoard[screen.width][screen.height], Automata readBoard[screen.width][screen.height])
{
    int x, y;
    int nextState;
    for (x = 1; x < screen.width; x = x + automataSize)
    {
        for (y = 1; y < screen.height; y = y + automataSize)
        {
            nextState = getAutomataNextState(readBoard[x][y], readBoard);
            writeBoard[x][y].state = nextState;
            writeBoard[x][y].color = (nextState == 1) ? YELLOW : BLACK;
        }
    }
}

void initBoard(Automata automataBoard[screen.width][screen.height])
{
    int x, y;
    for (x = 1; x < screen.width; x = x + automataSize)
    {
        for (y = 1; y < screen.height; y = y + automataSize)
        {
            int rand = GetRandomValue(1, 100);
            int initState = rand > 30 ? 0 : 1;
            Automata aut = {x, y, automataSize, automataSize, initState, (initState < 10) ? YELLOW : BLACK};
            automataBoard[x][y] = aut;
        }
    }
}

int main()
{
    InitWindow(screen.width, screen.height, "AUTOMATA");
    SetTargetFPS(fps);

    Automata(*automataBoard) = malloc(sizeof(Automata[screen.width][screen.height]));
    Automata(*automataBoardPrev) = malloc(sizeof(Automata[screen.width][screen.height]));
    initBoard(automataBoard);
    initBoard(automataBoardPrev);
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_P))
        {
            togglePause();
        }
        BeginDrawing();
        ClearBackground(screen.color);
        if (!paused)
        {
            Automata *tmp = automataBoardPrev;
            automataBoardPrev = automataBoard;
            automataBoard = tmp;
            computeNextBoard(automataBoard, automataBoardPrev);
            drawBoard(automataBoard);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}