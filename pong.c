#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <stdio.h>

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
    int accelerationX;
    int accelerationY;
    Color color;
} Automata;

int accelerationFactor = 10;
const Screen screen = {450, 800, 0, 450, 0, 800, BLACK};
char logText[50];

void setPosition(Automata *obj)
{
    obj->posX = obj->posX + obj->accelerationX;
    obj->posY = obj->posY + obj->accelerationY;

    if ((obj->posY + obj->width) >= screen.bottom)
    {
        obj->accelerationY = -accelerationFactor;
    }

    if ((obj->posY + obj->width) <= screen.top)
    {
        obj->accelerationY = accelerationFactor;
    }

    if ((obj->posX + obj->width) <= screen.left)
    {
        obj->accelerationX = accelerationFactor;
    }

    if ((obj->posX + obj->width) >= screen.right)
    {
        obj->accelerationX = -accelerationFactor;
    }
}

int main()
{

    Automata rect = {10, 10, 10, 10, accelerationFactor, accelerationFactor, GREEN};

    InitWindow(screen.width, screen.height, "RAYLIB");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(screen.color);
        DrawRectangle(rect.posX, rect.posY, rect.height, rect.width, rect.color);
        EndDrawing();

        setPosition(&rect);

        sprintf(logText, "x: %d y: %d ay; %d ax; %d", rect.posX, rect.posY, rect.accelerationY, rect.accelerationX);
        DrawText(logText, 5, 5, 1, WHITE);
    }

    CloseWindow();
    return 0;
}