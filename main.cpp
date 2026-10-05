/**
 * Author: <YOUR NAME HERE>
 * Assignment: Project 1 - Simple 2D Scene
 * Date due: 2026-10-05, 11:59 PM
 *
 * I certify that I completed this assignment independently
 * in accordance with the NYU School of Engineering Policies
 * and Procedures on Academic Misconduct.
 **/

// TODO: replace the header block above with the exact one from the
// assignment handout before submitting.

#include "raylib.h"
#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1200,
              SCREEN_HEIGHT = 675,
              FPS           = 60;

// Global Variables
AppStatus gAppStatus = RUNNING;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 1 - Simple 2D Scene");

    SetTargetFPS(FPS);
}

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {}

void render()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    EndDrawing();
}

void shutdown()
{
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
